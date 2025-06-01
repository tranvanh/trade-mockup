#pragma once

#include "Trade.h"
#include <functional>
#include <set>
#include <thread>

// namespace Application {
    class TradeApp;
// }

// TRADE_NAMESPACE_BEGIN

class TradeDatabase {
    // TradeApp& mApplication;
    struct {
        std::mutex                                                       lock;
        std::set<Trade, std::function<bool(const Trade&, const Trade&)>> data{ Trade::compareLowerPrice };
    } mTrades;

public:
    // TradeDatabase(TradeApp& app);
    void registerTrade(const Trade& trade);
    // void run();
};

// TRADE_NAMESPACE_END
