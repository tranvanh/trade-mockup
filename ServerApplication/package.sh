#!/bin/bash
CONFIG=${1:-Release}
BUILD=${2: yes}
CLEAN=${3:-no}  # Optional second parameter for cleaning

bash ./build.sh Release clean
cpack --config build/CPackConfig.cmake -C Release