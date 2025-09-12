#pragma once
#include "TradeLib/Trade.h"
#include <functional>
#include <set>
#include <thread>

TRANVANH_NAMESPACE_BEGIN

class TradeDatabase {
    struct {
        std::mutex                                                       lock;
        std::set<Trade, std::function<bool(const Trade&, const Trade&)>> data{ Trade::compareLowerPrice };
    } mTrades;

public:
    void registerTrade(const Trade& trade);
};

TRANVANH_NAMESPACE_END
