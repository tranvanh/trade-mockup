#!/bin/bash
CONFIG=${1:-Release}
CLEAN=${2:-no}  # Optional second parameter for cleaning

# Clean if requested
if [ "$CLEAN" = "clean" ]; then
    echo "Cleaning build directory..."
    rm -rf ./build
fi
echo "Configuring cmake..."
cmake -S . -B ./build -G "Ninja Multi-Config" -DBUILD_TESTS=OFF
echo "Building..."
cmake --build ./build --config "$CONFIG"
echo "Installing..."
cmake --install ./build --config "$CONFIG" --prefix="../includeLibs/"
