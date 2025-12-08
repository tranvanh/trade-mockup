#include "ServerApplication.h"
#include <UtilsLib/Logger.h>
#include <cxxopts.hpp>

// ### TODO
// - [x] Model generating buy/sell THREAD - give certain delay of buy/sell generation
// - [x] Mock database storing the information THREAD SAFE
// - [x] Matching trade/sell
// - [ ] Add trade database
// - [ ] Yield list of latest 100 trades happened around the given timestamp
// - [ ] Iterator to navigate the trades chronologicaly
// - [x] Create server-client connection
// - [x] Add commands and the input handling for client side
// - [x] Let client listen to happening trades
// - [ ] Error handling of connection/communication/parsing issues
// - [x] Heavy load handling, used clientApp with simulate currently crashes as the buffer communication buffer gets overwhelmed
// - [x] Handle multiple clients and handle reconnection
// - [ ] Handle failed connection with try to reconnect
// - [ ] Add UDP version of the server. Handle sequencing and packate order reliability
// - [x] Handle arguments properly
// - [x] Handle user inputs properly
// - [x] Logger
// - [x] Add thread pool
// - [ ] Add thread scheduler
// - [ ] Add multiple symbols to the stock market
// - [ ] Restructure UtilsLib folders
// - [x] Add google tests
// - [x] Add tests for Order book
// - [ ] Add tests for network sending/receiving, command parsing
// - [ ] Implement polling for macos and windows
// - [x] Assert, debug, release builds
// - [x] Add asserts
// - [x] Add Sandbox target
// - [x] Add Serialization utilities
// - [x] Add option to pre-fill the market with mock data
// - [x] Add FlatMap and use it in orderbook
// - [x] Add option for client to listen on to trades
// - [x] Add queries for more market

// Deduplicator
// * De-duplicates any message from the exchange based on its unique id.
// * Optimized for performance, uses floating window for history trades.

// Fills the provided OrderBook with a large number of randomly generated orders.
// Orders will have varying type (BUY/SELL), price, and volume.
// Note: This is defined in a .cpp on purpose to keep mock helpers out of the public headers.

int main(int argc, char* argv[]) {
    using namespace tranvanh;
    Logger::instance().setLevel(Logger::LogLevel::DEBUG);

    bool filled = false;
    try {
        cxxopts::Options options("ServerApp", "Example argument parsing");
        options.add_options()("filled", "Fill market with mock data", cxxopts::value<bool>())("h,help",
                                                                                                "Print help");
        const auto result = options.parse(argc, argv);
        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }
        if (result.count("filled")) {
            filled = result["filled"].as<bool>();
        }
    } catch (const std::exception& e) {
        std::cerr << "Argument parsing error: " << e.what() << "\n";
        return 1;
    }
    ServerApplication app(filled);
    app.run();
    return 0;
}
