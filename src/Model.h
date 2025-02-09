#pragma once

#include <string>


TRADE_API_NAMESPACE_BEGIN


enum class TradeType { BUY, SELL };

struct Trade {
    uint64_t    id;
    timepoint_t timeStamp;
    TradeType   tradeType;
    int price;
    int volume;

    friend std::ostream& operator<<(std::ostream& os, const Trade& trade);
    friend std::ostream &operator<<(std::ostream &os,
                         const timepoint_t &timepoint);
};


// must be thread safe queue
// producer consumer
// thread safe database
// - unordered map
class TradeDatabase{
    public:
    TradeDatabase() = default;
    // store all trades
    // search by id
    // search by timestamp
};

class TradeModel {
    TradeDatabase mDatabase;

public:
    TradeModel();
    void generateTrade(TradeType tradeType);
};

TRADE_API_NAMESPACE_END
