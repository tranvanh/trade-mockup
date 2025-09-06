#pragma once
#include "TradeLib/Trade.h"
#include <functional>
#include <set>
#include <thread>

// namespace Application {
class TradeApp;
// }

// TRADE_NAMESPACE_BEGIN

class TradeDatabase {
    struct {
        std::mutex                                                       lock;
        std::set<Trade, std::function<bool(const Trade&, const Trade&)>> data{ Trade::compareLowerPrice };
    } mTrades;

public:
    void registerTrade(const Trade& trade);
    // void run();
};

// TRADE_NAMESPACE_END
