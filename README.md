### Note:
Purpose of this project is to practice various technical topics as thread safety, networking, design patterns, heavy load handling, optimization. Overall goal is to build efficent mock trading platform having multiple clients connecting to one single server, acting as a market and processing the orders.

# Client Application
- Sends orders to server in json format
- Option to simulate order creations for stress testing

# Server Application
- Processes orders and delegades them to the market
- Serves as a communication layer between the market and the client

# TradeCore
- Trading specific components
- Order book with order matching algorithm. Matching highest buyer with lowest seller
- Stock market manager registering orders and trades

- **TradeCore.Test**
    - Using Google tests

- **TradeCore.Benchmark**
    - Using Google benchmark

# Toybox
- Fetched from [github.com/tranvanh/toybox](https://github.com/tranvanh/toybox) (tag `1.0`)

## How to build

This is a single CMake project. Configure and build from the root:

```bash
cmake -S . -B build -G "Ninja Multi-Config"
cmake --build build --config Release
```

Executables are placed in `build/ClientApplication/src/Release/ClientApplication` and `build/ServerApplication/src/Release/ServerApplication`.

`ClientApplication` accepts `<userId> [simulate]` — `<userId>` is any positive number, `simulate` optionally triggers automatic order generation.

Optional build flags:
- `-DBUILD_TESTS=ON` — builds TradeCore.Test (requires GTest, fetched automatically)
- `-DBUILD_BENCHMARK=ON` — builds TradeCore.Benchmark (requires Google Benchmark, fetched automatically)

### TODO
- [x] Model generating buy/sell THREAD - give certain delay of buy/sell generation
- [x] Mock database storing the information THREAD SAFE
- [x] Matching trade/sell
- [ ] Add trade database using SQL
- [ ] Yield list of latest 100 trades happened around the given timestamp
- [ ] Iterator to navigate the trades chronologicaly
- [x] Create server-client connection
- [x] Add commands and the input handling for client side
- [x] Let client listen to happening trades
- [ ] Error handling of connection/communication/parsing issues
- [x] Heavy load handling, used clientApp with simulate currently crashes as the buffer communication buffer gets overwhelmed
- [x] Handle multiple clients and handle reconnection
- [ ] Handle failed connection with try to reconnect
- [ ] Add UDP version of the server. Handle sequencing and packate order reliability
- [x] Handle arguments properly
- [x] Handle user inputs properly
- [x] Logger
- [x] Add thread pool
- [ ] Add thread scheduler
- [ ] Add multiple symbols to the stock market
- [x] Add google tests 
- [ ] Add tests for Order book, network sending/receiving, command parsing\
- [x] Assert, debug, release builds
- [x] Add asserts
- [x] Add Sandbox target
- [x] Add Serialization utilities
- [x] Add Sandbox target
- [x] Add Serialization utilities
- [x] Add option to pre-fill the market with mock data
- [x] Add FlatMap and use it in orderbook
- [x] Add option for client to listen on to trades
- [x] Add queries for more market
- [ ] Use profiler 
- [ ] Memory pool
- [x] Lock free queue
- [ ] High performance logger
- [x] Add GUI
