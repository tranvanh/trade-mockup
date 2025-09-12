#include "ServerApplication.h"
#include <iostream>
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
// - [ ] Handle multiple clients and handle reconnection, handle failed connection with try to reconnect
// - [x] Logger

// Deduplicator
// * De-duplicates any message from the exchange based on its unique id.
// * Optimized for performance, uses floating window for history trades.

int main() {
    // Logger::instance(Logger::LogLevel::INFO, "TEST");
    Logger::instance().setLevel(Logger::LogLevel::DEBUG);
    ServerApplication app;
    app.run();
    
    return 0;
}
