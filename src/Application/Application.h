#pragma once

#include "StockMarketGenerator.h"
#include "StockMarket.h"
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
    StockMarketGenerator mGenerator;
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
