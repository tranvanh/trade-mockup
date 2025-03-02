#pragma once

#include "Model.h"
#include "TradeBook.h"
#include <list>
#include <thread>
#include <functional>


TRADE_API_NAMESPACE_BEGIN

class TradeApp{
    TradeModel mModel;
    TradeBook mBook;
    std::list<std::thread> mThreadPool;

public:
    TradeApp();
    ~TradeApp();
    void run();
    void registerTrade(const Trade& trade);
    void runBackgroundTask(const std::function<void()>& f);

    std::atomic<bool>  isRunning = false;

};

TRADE_API_NAMESPACE_END
