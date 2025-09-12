#include "TradeLib/Trade.h"

TRANVANH_NAMESPACE_BEGIN

std::ostream& operator<<(std::ostream& os, const Trade& trade) {
    return os << "Trade:"
              << " buyer=" << std::setw(4) << trade.buyer.id << " seller=" << std::setw(4) << trade.seller.id
              << " price=" << std::setw(5) << trade.buyer.price << " volume=" << std::setw(4) << trade.volume;
}

bool Trade::compareLowerPrice(const Trade& t1, const Trade& t2) {
    return t1.tradeTime < t2.tradeTime;
}

TRANVANH_NAMESPACE_END
