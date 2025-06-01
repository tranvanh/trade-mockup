#pragma once

#include "OrderBook.h"
#include "TradeDatabase.h"
#include <atomic>

class StockMarket {
    OrderBook        mBook;
    TradeDatabase    mDatabase;
    std::atomic_bool mActive = false;

public:
    StockMarket()
        : mBook(*this){};
    ~StockMarket() { mActive = false; };

    void run();
    bool isActive() { return mActive; }

    void registerOrder(const Order& order);
    void registerTrade(const Trade& trade);
};