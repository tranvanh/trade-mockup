#pragma once
#include "TradeLib/OrderBook.h"
#include "TradeLib/TradeDatabase.h"
#include "UtilsLib/CallbackList.h"
#include <atomic>

class StockMarket {
    OrderBook        mBook;
    TradeDatabase    mDatabase;
    std::atomic_bool mActive = false;

    CallbackList<void(const Trade& trade)> mOnTradeCallbacks;

public:
    StockMarket()
        : mBook(*this){};
    ~StockMarket() { mActive = false; };

    void run();
    bool isActive() { return mActive; }

    void registerOrder(const Order& order);
    void registerTrade(const Trade& trade);

    [[nodiscard]] CallbackLifetime addOnTradeObserver(const std::function<void(const Trade& trade)>& callback);
};