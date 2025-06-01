#include "Order.h"
#include <chrono>

// TRADE_NAMESPACE_BEGIN

std::ostream& operator<<(std::ostream& os, OrderType type) {
    switch (type) {
    case (OrderType::SELL):
        os << "SELL";
        break;
    case (OrderType::BUY):
        os << "BUY";
        break;
    default:
        // ASSERT(false, "Trade type undeclared");
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Order& order) {
    return os << "Order("
              << "id=" << order.id << ","
              << " timestamp=" << order.timeStamp << ","
              << " price=" << order.price << ","
              << " volume=" << order.volume << ","
              << " type=" << order.type << ")";
}

// TRADE_NAMESPACE_END