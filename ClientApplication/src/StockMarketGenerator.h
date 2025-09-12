#pragma once
#include <TradeLib/Order.h>
#include <string>
#include <list>

// APPLICATION_NAMESPACE_BEGIN

class ClientApplication;

/// Simulates a trade market and generates orders
class StockMarketGenerator {
    ClientApplication& mApplication;

public:
    StockMarketGenerator(ClientApplication& app);
    void simulateMarket();
    
private:
    void generateOrder(OrderType tradeType);
};

// APPLICATION_NAMESPACE_END
