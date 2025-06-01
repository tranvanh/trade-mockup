#pragma once

#include <string>
#include <list>

TRADE_API_NAMESPACE_BEGIN

class TradeApp;

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
