#pragma once

#include <string>
#include <list>

TRADE_API_NAMESPACE_BEGIN

class TradeApp;

enum class TradeType { BUY, SELL };

struct Trade {
    uint64_t    id;
    timepoint_t timeStamp;
    TradeType   tradeType;
    int         price;
    int         volume;

    friend std::ostream& operator<<(std::ostream& os, const Trade& trade);
    friend std::ostream& operator<<(std::ostream& os, const timepoint_t& timepoint);

    static bool compareLowerPrice(const Trade& t1, const Trade& t2) { return t1.price < t2.price; }
    static bool compareGreaterPrice(const Trade& t1, const Trade& t2) { return t1.price > t2.price; }
};

/// Simulates a trade market and generates trades
class TradeModel {
    TradeApp& mApplication;
public:
    TradeModel(TradeApp& app);
    void simulate();
    
private:
    void generateTrade(TradeType tradeType);
};

TRADE_API_NAMESPACE_END
