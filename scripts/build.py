'''
Usage:
    python build.py --target windows
    python build.py --target android
'''

import sys
import os
import argparse
from detail import utils
import multiprocessing


def get_project_dir():
    script_dir = os.path.dirname(os.path.realpath(__file__))
    project_dir = os.path.join(script_dir, "..")
    project_dir = os.path.abspath(project_dir)
    return project_dir


def android_deploy(build_dir, install_dir):
    android_deployer = os.path.join(os.environ["QTKITS"], "android_armv7",
                                    "bin", "androiddeployqt.exe")
    utils.check_path(android_deployer)
    android_deployer = os.path.normpath(android_deployer)
    deployment_settings = os.path.join(
        build_dir, "android-libSensorServer.so-deployment-settings.json")
    deployment_settings = os.path.normpath(deployment_settings)
    platform = "android-28"
    jdk_path = os.environ["JAVA_HOME"]

    deploy_cmd = f"{android_deployer} --input {deployment_settings} --output {install_dir} --android-platform {platform} --jdk {jdk_path} --gradle"
    utils.execute(deploy_cmd.split())


def build_android_impl(buildtype):
    pro_file = os.path.join(os.getcwd(), "SensorServer.pro")
    ndk_make = os.path.join(os.environ["ANDROID_NDK_HOME"], "prebuilt",
                            "windows-x86_64", "bin", "make.exe")
    utils.check_path(ndk_make)

    os.environ["ANDROID_NDK_ROOT"] = os.environ["ANDROID_NDK_HOME"]
    os.environ["ANDROID_SDK_ROOT"] = os.environ["ANDROID_SDK_HOME"]
    os.environ["ANDROID_NDK_HOST"] = "windows-x86_64"
    qmake_path = os.path.join(os.environ["QTKITS"], "android_armv7", "bin",
                              "qmake.exe")

    output_dir = f"armeabi-v7a-android-{buildtype}"
    build_dir = os.path.join(os.getcwd(), "build", output_dir)
    install_dir = os.path.join(os.getcwd(), "installed", output_dir)

    utils.mkdir_p(build_dir)
    utils.mkdir_p(install_dir)

    qmake_generate = f"{qmake_path} {pro_file} -spec android-g++ CONFIG+={buildtype} CONFIG+=qtquickcompiler"
    qmake_all = f"{ndk_make} qmake_all"
    make_cmd = f"{ndk_make} -j{multiprocessing.cpu_count()}"
    make_install = f"{ndk_make} INSTALL_ROOT={install_dir} install"
    with utils.pushd(build_dir):
        utils.execute(qmake_generate.split())
        utils.execute(qmake_all.split())
        utils.execute(make_cmd.split())
        utils.execute(make_install.split())

    android_deploy(build_dir, install_dir)


def generate():
    gen_script = os.path.join(os.getcwd(), "scripts", "detail", "generate.py")
    utils.execute(f"python {gen_script}")


def build_android(args):
    project_dir = get_project_dir()
    with (utils.pushd(project_dir)):
        generate()
        build_android_impl("release")
        build_android_impl("debug")


def build_windows_impl(buildtype):
    msvc_env = R"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
    utils.check_path(msvc_env)
    pro_file = os.path.join(os.getcwd(), "SensorServer.pro")
    utils.check_path(pro_file)
    nmake_cmd = f"nmake {buildtype}"
    qmake_path = os.path.join(os.environ["QTKITS"], "msvc2017_64", "bin",
                              "qmake.exe")
    utils.check_path(qmake_path)

    output_dir = f"msvc-2017-x64-{buildtype}"
    build_dir = os.path.join(os.getcwd(), "build", output_dir)
    utils.mkdir_p(build_dir)

    qmake_generate_ = f"{qmake_path} {pro_file} -spec win32-msvc CONFIG+={buildtype} CONFIG+=qtquickcompiler"
    os.environ["CL"] = "/MP"
    with utils.pushd(build_dir):
        msvc_env = f"\"{msvc_env}\""
        build_cmd = msvc_env + " && " + qmake_generate_ + " && " + nmake_cmd
        build_cmd = build_cmd.replace("\r", "")
        build_cmd = build_cmd.replace("\n", "")
        utils.execute(build_cmd)


def build_windows(args):
    project_dir = get_project_dir()
    with (utils.pushd(project_dir)):
        generate()
        build_windows_impl("release")
        build_windows_impl("debug")


def build(args):
    if (args.target == "android"):
        build_android(args)
    elif (args.target == "windows"):
        build_windows(args)
    else:
        raise NotImplementedError(f"target {args.target} is not supported")


def main(argv):
    ap = argparse.ArgumentParser()
    ap.add_argument(
        "--target", required=True, help="Target platforms #:android #:windows")
    args = ap.parse_args()
    build(args)


if "__main__" == __name__:
    main(sys.argv)
