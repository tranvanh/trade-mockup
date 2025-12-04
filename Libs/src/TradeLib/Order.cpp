#include "TradeLib/Order.h"
#include "UtilsLib/Serialization.h"
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

std::ostream& Order::serialize(std::ostream& os) const{
    return os << "Order("
              << "id=" << clientId << ","
              << " timestamp=" << timeStamp << ","
              << " price=" << price << ","
              << " volume=" << volume << ","
              << " type=" << type << ")";
}

TRANVANH_NAMESPACE_END