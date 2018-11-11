import utils
import os
import sys

def main(argv):
    utils.cmd_exists("git")
    utils.cmd_exists("cmake")
    utils.check_path(os.environ["ANDROID_NDK_HOME"])
    utils.check_path(os.environ["ANDROID_SDK_HOME"])
    utils.check_path(os.environ["JAVA_HOME"])
    utils.check_path(os.environ["QTKITS"])
    utils.check_path(os.environ["QTDIR"])

if "__main__" == __name__:
    main(sys.argv)
