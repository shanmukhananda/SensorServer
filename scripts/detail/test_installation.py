import utils
import os
import sys
import argparse


def test_common_targets(args):
    utils.cmd_exists("git")
    utils.cmd_exists("cmake")
    utils.check_path(os.environ["QTDIR"])
    utils.check_path(os.environ["QTKITS"])


def test_windows_target(args):
    utils.check_path(os.path.join(os.environ["QTKITS"], "msvc2017_64"))
    msvc_env = R"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
    utils.check_path(msvc_env)
    qmake_path = os.path.join(os.environ["QTKITS"], "msvc2017_64", "bin",
                              "qmake.exe")
    utils.check_path(qmake_path)


def test_android_target(args):
    utils.check_path(os.environ["ANDROID_NDK_HOME"])
    utils.check_path(os.environ["ANDROID_SDK_HOME"])
    utils.check_path(os.environ["JAVA_HOME"])
    utils.check_path(os.path.join(os.environ["QTKITS"], "android_armv7"))
    ndk_make = os.path.join(os.environ["ANDROID_NDK_HOME"], "prebuilt",
                            "windows-x86_64", "bin", "make.exe")
    utils.check_path(ndk_make)
    android_deployer = os.path.join(os.environ["QTKITS"], "android_armv7",
                                    "bin", "androiddeployqt.exe")
    utils.check_path(android_deployer)
    qmake_path = os.path.join(os.environ["QTKITS"], "android_armv7", "bin",
                              "qmake.exe")
    utils.check_path(qmake_path)


def test_installation(args):
    test_common_targets(args)
    if (args.target == "android"):
        test_android_target(args)
    elif (args.target == "windows"):
        test_windows_target(args)
    else:
        raise NotImplementedError(f"target {args.target} is not supported")


def main(argv):
    ap = argparse.ArgumentParser()
    ap.add_argument(
        "--target", required=True, help="Target platforms #:android #:windows")
    args = ap.parse_args()
    test_installation(args)


if "__main__" == __name__:
    main(sys.argv)
