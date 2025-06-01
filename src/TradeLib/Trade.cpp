#include "Trade.h"

// TRADE_NAMESPACE_BEGIN

std::ostream& operator<<(std::ostream& os, const Trade& trade) {
    return os << "-----Trade-----" << "\n"
             << "Seller" << trade.seller << "\n"
             << "Buyer" << trade.buyer << "\n"
              << "time =" << trade.tradeTime << ","
              << "volume=" << trade.volume;
}

bool Trade::compareLowerPrice(const Trade& t1, const Trade& t2) { return t1.tradeTime < t2.tradeTime; }

// TRADE_NAMESPACE_END
