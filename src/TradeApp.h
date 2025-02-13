#pragma once

#include "Model.h"
#include "TradeDatabase.h"
#include <list>
#include <thread>
#include <functional>


TRADE_API_NAMESPACE_BEGIN

class TradeApp{
    TradeModel mModel;
    TradeDatabase mDatabase;
    std::list<std::thread> mThreadPool;
public:
    TradeApp();
    ~TradeApp();
    void run();
    void registerTrade(const Trade& trade);
    void runBackgroundTask(const std::function<void()>& f);
};

TRADE_API_NAMESPACE_END
