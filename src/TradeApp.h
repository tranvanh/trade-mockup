#pragma once

#include "Model.h"
#include <list>
#include <thread>

TRADE_API_NAMESPACE_BEGIN

class TradeModel;

class TradeApp{
    TradeModel mTradeModel;
    std::list<std::thread> mThreadPool;
public:
    TradeApp();
    ~TradeApp();
    void run();
};

TRADE_API_NAMESPACE_END
