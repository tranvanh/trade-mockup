# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build

This project requires **Ninja Multi-Config** — CMake will fatal-error on any other generator.

```bash
# Configure (first time or after CMakeLists changes)
cmake -S . -B build -G "Ninja Multi-Config"

# Build all targets (Release)
cmake --build build --config Release

# Build with tests
cmake -S . -B build -G "Ninja Multi-Config" -DBUILD_TESTS=ON
cmake --build build --config Debug

# Build with benchmarks
cmake -S . -B build -G "Ninja Multi-Config" -DBUILD_BENCHMARK=ON
cmake --build build --config Release
```

Available configs: `Debug`, `Release`, `RelWithDebInfo`, `Assert` (custom — enables asserts in release-like builds).

Compiler flags on TradeCore: `-Wall -Wextra -Wpedantic -Werror`.

## Run

```bash
# Server (optionally pre-fill market with 100 mock orders)
./build/ServerApplication/src/Release/ServerApplication [--filled]

# Client
./build/ClientApplication/src/Release/ClientApplication <userId> [simulate]
# userId: any positive integer
# simulate: optional flag to auto-generate orders (stress-test mode)
```

Server listens on port **8080**. Client connects to localhost.

## Tests

```bash
# Run all tests
cmake --build build --config Debug && ctest --test-dir build -C Debug

# Run a single test binary directly
./build/TradeCore.Test/Debug/TradeCore.Test --gtest_filter="OrderBook.*"
```

Tests use `TRANVANH_NAMESPACE_BEGIN/END` macros (from Toybox) instead of plain `namespace` blocks. GTest is fetched automatically by CMake when `BUILD_TESTS=ON`.

## Architecture

### Dependency graph

```
Toybox (external, fetched from github.com/tranvanh/toybox @ v1.0)
  └── TradeCore (static lib)
        ├── ServerApplication (executable)
        └── ClientApplication (executable)
TradeCore.Test      (links TradeCore + GTest)
TradeCore.Benchmark (links TradeCore + Google Benchmark)
TradeCore.Sandbox   (scratch target)
```

External deps fetched via `FetchContent`: **Toybox**, **nlohmann/json** (v3.11.3), **cxxopts** (v3.3.1), **Boost** (system-installed, `find_package`).

### TradeCore

The trading engine, built as a static library. Key types:

- **`Order`** (`Order.h`) — `{clientId, OrderType(BUY/SELL), price, volume, timestamp}`. Extends `toybox::Serializable`.
- **`Trade`** (`Trade.h`) — result of a match: `{buyerId, sellerId, price, volume}`.
- **`OrderBook`** (`OrderBook.h`) — single-symbol order book. Accepts orders from any thread via `registerOrder()` (pushed onto a `ThreadSafeQueue`), then processes them synchronously on one thread when `pollOrders()` is called. Maintains separate `FlatMap<price → PriceLevel>` for buyers (highest first) and sellers (lowest first). Emits matched trades through `onTradeCallbacks` (a `CallbackList`).
- **`Market`** (`Market.h`) — owns an `OrderBook`, runs a polling loop in a background thread (`run()`), exposes `registerOrder()` and `addOnTradeObserver()`.

Matching rule: `trade.price = min(requester.price, resting.price)`. Priority: best price first, then FIFO within a price level.

### ServerApplication

Owns a `Market` and a `toybox::Server` (TCP, port 8080). Incoming JSON messages are dispatched to background tasks (thread pool via `toybox::Application`) which parse the order and call `market.registerOrder()`. Inherits `toybox::CallbackOwner` to hold trade-observer lifetimes.

JSON wire format for orders:
```json
{ "clientId": 1, "type": 0, "price": 100, "volume": 10 }
```
`type`: `0` = BUY, `1` = SELL.

### ClientApplication

Owns a `toybox::Client` and a `StockMarketGenerator`. Two threads (`THREAD_COUNT = 2`): one for user input (`processUserInputs`), one for optional simulation (`StockMarketGenerator::simulateMarket`). Commands: `buy <price> <volume>`, `sell <price> <volume>`, `exit`.

### Toybox

A personal utility library (`github.com/tranvanh/toybox @ v1.0`) providing: `ThreadSafeQueue`, `FlatMap`, `CallbackList`/`CallbackLifetime`/`CallbackOwner`, `Server`/`Client` (TCP), `Application` (thread pool), `Logger`, `Serialization`.
