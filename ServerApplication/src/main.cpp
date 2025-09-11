#include "TradeApp.h"
#include <iostream>
#include <UtilsLib/Logger.h>

// Trades TODO
// [x] model generating buy/sell THREAD - give certain delay of buy/sell generatio -> producer consumer
// [x] mock database storing the information THREAD SAFE
// [x] matching trade/sell
// [ ] yield list of latest 100 trades happened around the given timestamp
// [ ] iterator to navigate the trades chronologicaly
// [ ] create server-client connection
// [ ] add commands
// [ ] error handling
//
// [x] Logger, of happened trades vs already happenning trades
// -> logger for nice output

// Deduplicator
// * De-duplicates any message from the exchange based on its unique id.
// * Optimized for performance, uses floating window for history trades.

int main() {
    // Logger::instance(Logger::LogLevel::INFO, "TEST");
    Logger::instance().setLevel(Logger::LogLevel::DEBUG);
    TradeApp app;
    app.run();
    
    return 0;
}



// int main() {
//     Server server(Server::AddressType::ANY);
//     server.openSocket();
//     Logger::instance().setLevel(Logger::LogLevel::INFO);
//     server.startListen(8080, [](const SocketData data){
//         Logger::instance().log(Logger::LogLevel::DEBUG, "msg received");
//         std::string test(data.buffer, data.size);
//         std::cout << test << std::endl;
//     });
//     return 1;
// }