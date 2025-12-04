#include "TradeLib/Trade.h"

TRANVANH_NAMESPACE_BEGIN

std::ostream& Trade::serialize(std::ostream& os) const {
    return os << "Trade:"
              << " buyer=" << std::setw(4) << buyer.clientId << " seller=" << std::setw(4) << seller.clientId
              << " price=" << std::setw(5) << buyer.price << " volume=" << std::setw(4) << volume;
}

bool Trade::compareLowerPrice(const Trade& t1, const Trade& t2) {
    return t1.tradeTime < t2.tradeTime;
}

TRANVANH_NAMESPACE_END
