#pragma once

#include "UtilsLib/TimePointUtils.h"
#include <string>

// TRADE_NAMESPACE_BEGIN

constexpr int PRICE_MAX  = 10000;
constexpr int VOLUME_MAX = 1000;

enum class OrderType { BUY, SELL };

std::ostream& operator<<(std::ostream& os, OrderType type);

// std::ostream& operator<<(std::ostream& os, OrderType type) {
//     switch (type) {
//     case (OrderType::SELL):
//         os << "SELL";
//         break;
//     case (OrderType::BUY):
//         os << "BUY";
//         break;
//     default:
//         // ASSERT(false, "Trade type undeclared");
//         break;
//     }
//     return os;
// }

struct Order {
    uint64_t    id;
    timepoint_t timeStamp;
    OrderType   type;
    int         price;
    int         volume;

    friend std::ostream& operator<<(std::ostream& os, const Order& trade);
};

// TRADE_NAMESPACE_END
