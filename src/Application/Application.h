#pragma once

#include "MarketGenerator.h"
#include "TradeDatabase.h"
#include "OrderBook.h"
#include <list>
#include <thread>
#include <atomic>
#include <functional>
#include <Order.h>
#include <Trade.h>

// APPLICATION_NAMESPACE_BEGIN

class TradeApp{
    MarketGenerator mMarket;
    OrderBook mBook;
    TradeDatabase mDatabase;
    std::list<std::thread> mThreadPool;

public:
    TradeApp();
    ~TradeApp();
    
    void run();
    void registerOrder(const Order& trade);
    void registerTrade(const Trade& trade);
    void runBackgroundTask(const std::function<void()>& f);

    std::atomic<bool>  isRunning;
};

// APPLICATION_NAMESPACE_END
