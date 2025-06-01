#pragma once

#include <string>
#include "TimePointUtils.h"

// TRADE_NAMESPACE_BEGIN

enum class OrderType { BUY, SELL };

std::ostream& operator<<(std::ostream& os, const timepoint_t& timepoint);

struct Order {
    uint64_t    id;
    timepoint_t timeStamp;
    OrderType   type;
    int         price;
    int         volume;

    friend std::ostream& operator<<(std::ostream& os, const Order& trade);
};

// TRADE_NAMESPACE_END
