#include "ServerApplication.h"
#include <UtilsLib/Common.h>
#include <UtilsLib/Logger.h>

// ### TODO
// - [x] Model generating buy/sell THREAD - give certain delay of buy/sell generation
// - [x] Mock database storing the information THREAD SAFE
// - [x] Matching trade/sell
// - [ ] Yield list of latest 100 trades happened around the given timestamp
// - [ ] Iterator to navigate the trades chronologicaly
// - [x] Create server-client connection
// - [x] Add commands and the input handling for client side
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
// - [ ] Add tests for Order book, network sending/receiving, command parsing
// - [ ] Implement polling for macos and windows
// - [x] Assert, debug, release builds
// - [x] Add asserts

// Deduplicator
// * De-duplicates any message from the exchange based on its unique id.
// * Optimized for performance, uses floating window for history trades.

int main() {
    using namespace tranvanh;
    Logger::instance().setLevel(Logger::LogLevel::DEBUG);
    ServerApplication app;
    app.run();
    return 0;
}
