#pragma once

#include "Model.h"
#include "TradeDatabase.h"
#include "OrderBook.h"
#include <list>
#include <thread>
#include <functional>


TRADE_API_NAMESPACE_BEGIN

class TradeApp{
    OrderModel mModel;
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

    std::atomic<bool>  isRunning = false;

};

TRADE_API_NAMESPACE_END
