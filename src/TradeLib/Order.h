#pragma once

#include <string>
#include "TimePointUtils.h"

// TRADE_NAMESPACE_BEGIN

constexpr int PRICE_MAX = 10000;
constexpr int VOLUME_MAX = 1000;

enum class OrderType { BUY, SELL };

std::ostream& operator<<(std::ostream& os, const timepoint_t& timepoint);
std::ostream& operator<<(std::ostream& os, OrderType type);
struct Order {
    uint64_t    id;
    timepoint_t timeStamp;
    OrderType   type;
    int         price;
    int         volume;

    friend std::ostream& operator<<(std::ostream& os, const Order& trade);
};

// TRADE_NAMESPACE_END
