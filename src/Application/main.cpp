#include "TradeApp.h"
#include <iostream>

// Trades
// [x] model generating buy/sell THREAD - give certain delay of buy/sell generatio -> producer consumer
// [x] mock database storing the information THREAD SAFE
// [x] matching trade/sell
// [ ] yield list of latest 100 trades happened around the given timestamp
// [ ] iterator to navigate the trades chronologicaly
//
// * Logger, of happened trades vs already happenning trades
// -> logger for nice output

// Deduplicator
// * De-duplicates any message from the exchange based on its unique id.
// * Optimized for performance, uses floating window for history trades.

int main() {
    TradeApp app;
    app.run();
    return 0;
}