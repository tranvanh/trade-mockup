#!/bin/bash
CONFIG=${1:-Release}
CLEAN=${2:-no}  # Optional second parameter for cleaning

# Clean if requested
if [ "$CLEAN" = "clean" ]; then
    echo "Cleaning build directory..."
    rm -rf ./build
fi

cmake -S . -B ./build -G "Ninja Multi-Config" -DBUILD_TESTS=ON
cmake --build ./build --config "$CONFIG"
ctest --test-dir ./build --config "$CONFIG" --output-on-failure
