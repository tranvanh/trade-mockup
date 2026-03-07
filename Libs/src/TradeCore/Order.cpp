#include "TradeCore/Order.h"
#include "Toybox/Serialization.h"

namespace TradeCore {
    std::ostream& Order::serialize(std::ostream& os) const{
        return os << "Order("
                  << "id=" << clientId << ","
                  << " timestamp=" << timeStamp << ","
                  << " price=" << price << ","
                  << " volume=" << volume << ","
                  << " type=" << type << ")";
    }
}

std::ostream& operator<<(std::ostream& os, TradeCore::OrderType type) {
    switch (type) {
    case (TradeCore::OrderType::SELL):
        os << "SELL";
        break;
    case (TradeCore::OrderType::BUY):
        os << "BUY";
        break;
    default:
        ASSERT(false, "Trade type undeclared");
        break;
    }
    return os;
}
