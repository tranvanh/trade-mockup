#include "TradeLib/Order.h"
#include <chrono>

TRANVANH_NAMESPACE_BEGIN

std::ostream& operator<<(std::ostream& os, OrderType type) {
    switch (type) {
    case (OrderType::SELL):
        os << "SELL";
        break;
    case (OrderType::BUY):
        os << "BUY";
        break;
    default:
        ASSERT(false, "Trade type undeclared");
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

TRANVANH_NAMESPACE_END