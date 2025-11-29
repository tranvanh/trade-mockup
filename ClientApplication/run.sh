#!/bin/bash
CONFIG=${1:-Release}
BUILD=${2: yes}
CLEAN=${3:-no}  # Optional second parameter for cleaning

if [ "$BUILD" = "build" ]; then
    bash ./build.sh "$CONFIG" "$CLEAN"
fi

./build/src/"$CONFIG"/serverApp
