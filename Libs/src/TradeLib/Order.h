#pragma once

#include "UtilsLib/TimePointUtils.h"
#include <string>

// TRADE_NAMESPACE_BEGIN

constexpr int PRICE_MAX  = 10000;
constexpr int VOLUME_MAX = 1000;

enum class OrderType { BUY = 0, SELL = 1};

std::ostream& operator<<(std::ostream& os, OrderType type);

struct Order {
    uint64_t    id = 0;
    timepoint_t timeStamp;
    OrderType   type;
    int         price;
    int         volume;

    friend std::ostream& operator<<(std::ostream& os, const Order& trade);
};

// TRADE_NAMESPACE_END
