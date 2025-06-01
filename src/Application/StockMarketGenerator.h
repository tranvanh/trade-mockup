#pragma once

#include "StockMarket.h"
#include "Order.h"
#include <string>
#include <list>

// APPLICATION_NAMESPACE_BEGIN

class TradeApp;

/// Simulates a trade market and generates orders
class StockMarketGenerator {
    TradeApp& mApplication;
    StockMarket& mStockMarket;

public:
    StockMarketGenerator(TradeApp& app);
    void simulateMarket();
    
private:
    void generateOrder(OrderType tradeType);
};

// APPLICATION_NAMESPACE_END
