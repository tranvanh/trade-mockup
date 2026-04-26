### Note:
Purpose of this project is to practice various technical topics as thread safety, networking, design patterns, heavy load handling, optimization. Overall goal is to build efficent mock trading platform having multiple clients connecting to one single server, acting as a market and processing the orders.

# Client Application
- Full-screen terminal UI (FTXUI) — command history panel on the left, trade feed on the right
- Commands: `subscribe`, `buy <price> <volume>`, `sell <price> <volume>`, `simulate` (toggle auto-generation)
- Sends orders to the server in JSON format over TCP
- Simulation mode generates bursts of random buy/sell orders for stress testing

# Server Application
- Full-screen terminal UI — active connections panel, trade feed, and message log
- Processes incoming orders and forwards them to the market engine
- Broadcasts matched trades to subscribed clients
- Serves as the communication layer between the market and clients

# TradeGUI
- Shared FTXUI component library used by both applications
- Thread-safe `UIState` with per-field mutex-guarded deques
- `UIStream` — custom `std::ostream` that redirects `Logger` output into the UI without touching stdout
- Panel factories: trade feed, log, connections, command history

# TradeCore
- Trading engine, built as a static library
- `OrderBook` — single-symbol order book with price-time priority matching. Highest buyer matched with lowest seller; trade price is the resting order's price
- `Market` — owns an `OrderBook`, runs its polling loop on a background thread, exposes a trade observer API

- **TradeCore.Test**
    - Unit tests using Google Test

- **TradeCore.Benchmark**
    - Micro-benchmarks using Google Benchmark

# Toybox
- Fetched from [github.com/tranvanh/toybox](https://github.com/tranvanh/toybox) (tag `1.0`)
- Provides: `ThreadSafeQueue`, `FlatMap`, `CallbackList`/`CallbackLifetime`/`CallbackOwner`, `Server`/`Client` (TCP, Boost.Asio), `Application` (thread pool), `Logger`, `Serialization`

## How to build

Requires **Ninja Multi-Config** — CMake will error on any other generator.

```bash
cmake -S . -B build -G "Ninja Multi-Config"
cmake --build build --config Release
```

Optional build flags:
- `-DBUILD_TESTS=ON` — builds TradeCore.Test (GTest fetched automatically)
- `-DBUILD_BENCHMARK=ON` — builds TradeCore.Benchmark (Google Benchmark fetched automatically)

## How to run

```bash
# Terminal 1 — start the server
./build/ServerApplication/src/Release/ServerApplication

# Terminal 2 — start a client
./build/ClientApplication/src/Release/ClientApplication
```

Server listens on port **8080**. The client connects to `127.0.0.1:8080` automatically on startup.

**Client TUI commands:**
| Command | Effect |
|---|---|
| `subscribe` | Subscribe to live trade broadcasts from the server |
| `buy <price> <volume>` | Place a buy order |
| `sell <price> <volume>` | Place a sell order |
| `simulate` | Toggle automatic order generation (stress-test mode) |
| `Esc` | Quit |

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
