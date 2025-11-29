#!/bin/bash
CONFIG=${1:-Release}
CLEAN=${2:-no}

# Clean if requested
if [ "$CLEAN" = "clean" ]; then
    echo "Cleaning build directory..."
    rm -rf ./build
fi
echo "Configuring cmake..."
cmake -S . -B ./build -G "Ninja Multi-Config"
echo "Building..."
cmake --build ./build --config "$CONFIG"
echo "Build succesful!"
