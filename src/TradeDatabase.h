#pragma once

#include <set>
#include <thread>
#include <functional>
#include "Model.h"

TRADE_API_NAMESPACE_BEGIN

class TradeApp;

struct Trade {
    Order       seller;
    Order       buyer;
    timepoint_t tradeTime;
    int         volume;

    friend std::ostream& operator<<(std::ostream& os, const Trade& trade);
    static bool compareLowerPrice(const Trade& t1, const Trade& t2) { return t1.tradeTime < t2.tradeTime; }
};


class TradeDatabase {
    TradeApp&                           mApplication;
    struct{
        std::mutex lock;
        std::set<Trade, std::function<bool(const Trade&, const Trade&)>> data{Trade::compareLowerPrice};
    } mTrades;

public:
TradeDatabase(TradeApp& app);
    void registerTrade(const Trade& trade);
    void run();
private:

};

TRADE_API_NAMESPACE_END