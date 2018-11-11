#/bin/bash

echo "-------------------------------------------------------------------------"
echo "~ EXTERNAL DEPENDENCIES ~"
echo "  # clang-format"
echo "      - apt update && apt install clang-format -y"
echo "-------------------------------------------------------------------------"

script_dir=$(realpath $(dirname $0))
project_dir=$(realpath ${script_dir}/../../..)

function format() {
    directory=${1}
    find ${directory} \
        -name '*.cpp' \
        -o -name '*.hpp' \
        -o -name '*.cc' \
        -o -name '*.hh' \
        -o -name '*.c' \
        -o -name '*.h' \
        | xargs clang-format -i -style=file
}

function main() {
    format ${project_dir}/app
    format ${project_dir}/experiments
}

main
