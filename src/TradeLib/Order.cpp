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


std::ostream& operator<<(std::ostream& os, const timepoint_t& timepoint) {
    auto tp = std::chrono::system_clock::to_time_t(timepoint);
    return os << std::put_time(std::localtime(&tp), "%F %T");
}

// TRADE_NAMESPACE_END