#pragma once

#include "Common.h"

TRADE_API_NAMESPACE_BEGIN

class TradeModel;

class TradeApp{
    TradeModel& tradeModel;
public:
    TradeApp();
    void run();
};

TRADE_API_NAMESPACE_END
