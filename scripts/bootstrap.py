'''
Usage:
    python bootstrap.py --target windows
    python bootstrap.py --target android
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


def create_vendor_dirs():
    vendor_dir = os.path.join(os.getcwd(), "vendor")
    utils.mkdir_p(os.path.join(vendor_dir, "downloads"))
    utils.mkdir_p(os.path.join(vendor_dir, "installed"))
    utils.mkdir_p(os.path.join(vendor_dir, "buildtrees"))


def download_packages():
    dwld_dir = os.path.join("vendor", "downloads")
    with utils.pushd(dwld_dir):
        if not os.path.exists("flatbuffers"):
            cmd = "git clone --recursive https://github.com/google/flatbuffers.git"
            utils.execute(cmd.split())


def build_flatbuffer_android(buildtype):
    project_dir = os.getcwd()
    flb_dir = os.path.join("vendor", "downloads", "flatbuffers")
    flb_dir = os.path.join(project_dir, flb_dir)
    output_dir = f"armeabi-v7a-android-{buildtype}"

    install_dir = os.path.join(project_dir, "vendor", "installed", output_dir)
    build_dir = os.path.join(project_dir, "vendor", "buildtrees",
                             "flatbuffers", output_dir)

    utils.mkdir_p(install_dir)
    utils.mkdir_p(build_dir)

    android_toolchain = os.path.join(os.environ["ANDROID_NDK_HOME"], "build",
                                     "cmake", "android.toolchain.cmake")
    android_make = os.path.join(os.environ["ANDROID_NDK_HOME"], "prebuilt",
                                "windows-x86_64", "bin", "make.exe")

    cmake_generate = [
        "cmake", "-G", "MinGW Makefiles", "-DANDROID_ABI=armeabi-v7a",
        "-DANDROID_NATIVE_API_LEVEL=android-16",
        "-DANDROID_PLATFORM=android-16", f"-DCMAKE_BUILD_TYPE={buildtype}",
        f"-DCMAKE_INSTALL_PREFIX={install_dir}",
        f"-DCMAKE_MAKE_PROGRAM={android_make}",
        f"-DCMAKE_TOOLCHAIN_FILE={android_toolchain}",
        "-DFLATBUFFERS_BUILD_TESTS=OFF", "-DFLATBUFFERS_BUILD_FLATC=OFF",
        "-DFLATBUFFERS_BUILD_FLATHASH=OFF", flb_dir
    ]

    cores = multiprocessing.cpu_count()

    cmake_build = [
        "cmake", "--build", build_dir, "--config", buildtype, "--target",
        "install", "--", f"-j{cores}"
    ]

    with utils.pushd(build_dir):
        utils.execute(cmake_generate)
        utils.execute(cmake_build)


def build_flatbuffer_windows(buildtype):
    project_dir = os.getcwd()
    flb_dir = os.path.join("vendor", "downloads", "flatbuffers")
    flb_dir = os.path.join(project_dir, flb_dir)
    output_dir = f"msvc-2017-x64-windows-{buildtype}"

    install_dir = os.path.join(project_dir, "vendor", "installed", output_dir)
    build_dir = os.path.join(project_dir, "vendor", "buildtrees",
                             "flatbuffers", output_dir)

    utils.mkdir_p(install_dir)
    utils.mkdir_p(build_dir)

    cmake_generate = [
        "cmake", "-G", "Visual Studio 15 2017 Win64",
        f"-DCMAKE_BUILD_TYPE={buildtype}",
        f"-DCMAKE_INSTALL_PREFIX={install_dir}",
        "-DFLATBUFFERS_BUILD_TESTS=OFF", "-DFLATBUFFERS_BUILD_FLATC=ON",
        "-DFLATBUFFERS_BUILD_FLATHASH=OFF", flb_dir
    ]

    cmake_build = [
        "cmake", "--build", build_dir, "--config", buildtype, "--target",
        "install"
    ]

    with utils.pushd(build_dir):
        utils.execute(cmake_generate)
        utils.execute(cmake_build)


def build_packages_for_windows():
    build_flatbuffer_windows("release")
    build_flatbuffer_windows("debug")


def bootstrap_android(args):
    build_flatbuffer_android("release")
    build_flatbuffer_android("debug")


def bootstrap_windows(args):
    project_dir = get_project_dir()
    with (utils.pushd(project_dir)):
        create_vendor_dirs()
        download_packages()
        build_packages_for_windows()


def bootstrap(args):
    if (args.target == "android"):
        bootstrap_android(args)
    elif (args.target == "windows"):
        bootstrap_windows(args)
    else:
        raise NotImplementedError(f"target {args.target} is not supported")


def main(argv):
    ap = argparse.ArgumentParser()
    ap.add_argument(
        "--target", required=True, help="Target platforms #:android #:windows")
    args = ap.parse_args()
    bootstrap(args)


if "__main__" == __name__:
    main(sys.argv)
