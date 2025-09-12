#include "ServerApplication.h"
#include <iostream>
#include <UtilsLib/Logger.h>

// Trades TODO
// [x] model generating buy/sell THREAD - give certain delay of buy/sell generatio -> producer consumer
// [x] mock database storing the information THREAD SAFE
// [x] matching trade/sell
// [ ] yield list of latest 100 trades happened around the given timestamp
// [ ] iterator to navigate the trades chronologicaly
// [x] create server-client connection
// [ ] add commands
// [ ] error handling
// [ ] Heavy load handling, used with simulate (show processes = "top" then "kill -9 <PID>")
// [ ] Handle multiple clients and handle reconnection, handle failed connection

//
// [x] Logger, of happened trades vs already happenning trades
// -> logger for nice output

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
