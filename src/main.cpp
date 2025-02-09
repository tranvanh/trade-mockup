#include <iostream>
#include "Common.h"
#include "Model.h"

// Trades
// * model generating buy/sell THREAD - give certain delay of buy/sell generatio -> producer consumer
// * mock database storing the information THREAD SAFE
// * matching trade/sell
// * yield list of latest 100 trades happened around the given timestamp
// * iterator to navigate the trades chronologicaly
//
//
// * Logger, of happened trades vs already happenning trades

// Deduplicator
// * De-duplicates any message from the exchange based on its unique id.
// * Optimized for performance, uses floating window for history trades.

int main() {

    TradeApi::TradeModel tradeModel;
    tradeModel.run();
    std::cout << "hello test test" << std::endl;
    return 0;
}