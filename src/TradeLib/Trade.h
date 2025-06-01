#pragma once
#include "Order.h"
#include "TimePointUtils.h"

// TRADE_NAMESPACE_BEGIN

struct Trade {
    Order       seller;
    Order       buyer;
    timepoint_t tradeTime;
    int         volume;

    friend std::ostream& operator<<(std::ostream& os, const Trade& trade);
    static bool compareLowerPrice(const Trade& t1, const Trade& t2);
};

// TRADE_NAMESPACE_END
