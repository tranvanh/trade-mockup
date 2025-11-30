#!/bin/bash
CONFIG=${1:-Release}

echo "Cleaning build directory..."
rm -rf ./build

cmake -S . -B ./build -G "Ninja Multi-Config" -DBUILD_TESTS=ON
cmake --build ./build --config "$CONFIG"
ctest --test-dir ./build --config "$CONFIG" --output-on-failure
