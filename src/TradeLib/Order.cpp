#include "TradeLib/Order.h"
#include <chrono>

std::ostream& operator<<(std::ostream& os, const Order& order) {
    return os << "Order("
              << "id=" << order.id << ","
              << "timestamp=" << order.timeStamp << ","
              << "price=" << order.price << ","
              << "volume=" << order.volume << ","
              << "type=" << order.type << ")";
}


std::ostream& operator<<(std::ostream& os, const timepoint_t& timepoint) {
    auto tp = std::chrono::system_clock::to_time_t(timepoint);
    return os << std::put_time(std::localtime(&tp), "%F %T");
}

std::ostream& operator<<(std::ostream& os, OrderType type) {
    switch (type) {
    case (OrderType::SELL):
        os << "SELL";
        break;
    case (OrderType::BUY):
        os << "BUY";
        break;
    default:
        ASSERT(true, "Trade type undeclared");
    }
    return os;
}
