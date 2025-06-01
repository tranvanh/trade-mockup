#pragma once

#include "Order.h"
#include <string>
#include <list>

// APPLICATION_NAMESPACE_BEGIN

class TradeApp;

/// Simulates a trade market and generates orders
class MarketGenerator {
    TradeApp& mApplication;
public:
MarketGenerator(TradeApp& app);
    void simulateMarket();
    
private:
    void generateOrder(OrderType tradeType);
};

// APPLICATION_NAMESPACE_END
