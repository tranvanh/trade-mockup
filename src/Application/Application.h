#pragma once

#include "StockMarketGenerator.h"
#include "StockMarket.h"
#include "CallbackOwner.h"

#include <forward_list>
#include <thread>
#include <atomic>
#include <functional>
#include <Order.h>
#include <Trade.h>

// APPLICATION_NAMESPACE_BEGIN

class TradeApp : CallbackOwner{
    StockMarketGenerator mGenerator;
    StockMarket mStockMarket;
    std::forward_list<std::thread> mThreadPool;

public:
    TradeApp();
    ~TradeApp();
    
    void run();
    void registerOrder(const Order& trade);
    void registerTrade(const Trade& trade);
    void runBackgroundTask(const std::function<void()>& f);
    StockMarket& getStockMarket();

    std::atomic<bool>  isRunning;
};

// APPLICATION_NAMESPACE_END
