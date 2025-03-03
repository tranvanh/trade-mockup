#pragma once

#include <string>
#include <list>

TRADE_API_NAMESPACE_BEGIN

class TradeApp;

enum class OrderType { BUY, SELL };

std::ostream& operator<<(std::ostream& os, const timepoint_t& timepoint);

struct Order {
    uint64_t    id;
    timepoint_t timeStamp;
    OrderType   type;
    int         price;
    int         volume;

    friend std::ostream& operator<<(std::ostream& os, const Order& trade);
};

/// Simulates a trade market and generates orders
class OrderModel {
    TradeApp& mApplication;
public:
OrderModel(TradeApp& app);
    void simulateMarket();
    
private:
    void generateOrder(OrderType tradeType);
};

TRADE_API_NAMESPACE_END
