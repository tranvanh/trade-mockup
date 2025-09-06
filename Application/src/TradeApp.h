#pragma once
#include "UtilsLib/CallbackOwner.h"
#include "DumbInvestor.h"
#include "TradeLib/StockMarket.h"
#include "StockMarketGenerator.h"
#include <atomic>
#include <forward_list>
#include <functional>
#include <TradeLib/Order.h>
#include <thread>
#include <TradeLib/Trade.h>

// APPLICATION_NAMESPACE_BEGIN

class TradeApp : CallbackOwner {
    StockMarketGenerator mGenerator;
    StockMarket          mStockMarket;

    DumbInvestor mInvestor;

    std::forward_list<std::thread> mThreadPool;

public:
    TradeApp();
    ~TradeApp();

    void         run();
    void         registerOrder(const Order& trade);
    void         registerTrade(const Trade& trade);
    void         runBackgroundTask(const std::function<void()>& f);
    StockMarket& getStockMarket();

    std::atomic<bool> isRunning = false;
};

// APPLICATION_NAMESPACE_END
