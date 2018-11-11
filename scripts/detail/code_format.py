import utils
import subprocess
import os
import sys

def get_project_dir():
    script_dir = os.path.dirname(os.path.realpath(__file__))
    project_dir = os.path.join(script_dir, "..", "..")
    project_dir = os.path.abspath(project_dir)
    return project_dir

def clang_format(directory):
    for root, dirs, files in os.walk(directory):
        for filename in files:
            if filename.endswith(('.cpp', '.hpp', '.cc', '.hh', '.c', '.h')):
                filepath = os.path.join(root, filename)
                format_cmd = f"clang-format -i -style=file {filepath}"
                utils.execute(format_cmd.split())

def main(argv):
    project_dir = get_project_dir()
    
    with utils.pushd(project_dir):
        clang_format(os.path.join(project_dir, "app"))
        clang_format(os.path.join(project_dir, "experiments"))

if "__main__" == __name__:
    main(sys.argv)
