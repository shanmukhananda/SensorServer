import os
import subprocess
import utils


def get_project_dir():
    script_dir = os.path.dirname(os.path.realpath(__file__))
    project_dir = os.path.join(script_dir, "..", "..")
    project_dir = os.path.abspath(project_dir)
    return project_dir


def main():
    project_dir = get_project_dir()
    flatc_path = os.path.join(project_dir, "vendor", "installed",
                              "msvc-2017-x64-windows-release", "bin",
                              "flatc.exe")
    msg_fbs = os.path.join(project_dir, "app", "messages", "messages.fbs")
    udp_hdr_fbs = os.path.join(project_dir, "app", "messages", "udpheader.fbs")

    utils.check_path(flatc_path)
    utils.check_path(msg_fbs)
    utils.check_path(udp_hdr_fbs)

    gen_msgs = f"{flatc_path} --cpp {msg_fbs}"
    gen_udp_hdr = f"{flatc_path} --cpp {udp_hdr_fbs}"

    with utils.pushd(os.path.join(project_dir, "app", "messages")):
        utils.execute(gen_msgs.split())
        utils.execute(gen_udp_hdr.split())


if "__main__" == __name__:
    main()
