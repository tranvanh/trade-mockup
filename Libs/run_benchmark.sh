#!/bin/bash
CONFIG=${1:-Release}

echo "Cleaning build directory..."
rm -rf ./build

cmake -S . -B ./build -G "Ninja Multi-Config" -DBUILD_BENCHMARK=ON
cmake --build ./build --config "$CONFIG"
./build/src/TradeLib.Benchmark/"$CONFIG"/TradeLib.Benchmark
