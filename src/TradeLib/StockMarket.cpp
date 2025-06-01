#include "StockMarket.h"

void StockMarket::run() {
    mActive = true;
    mBook.run();
}

void StockMarket::registerOrder(const Order& order) {
    mBook.registerOrder(order);
}

void StockMarket::registerTrade(const Trade& trade) {
    mDatabase.registerTrade(trade);
}