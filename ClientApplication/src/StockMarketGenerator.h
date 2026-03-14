#pragma once
#include <TradeCore/Order.h>

class ClientApplication;

/// Simulates a trade market and generates orders
class StockMarketGenerator {
    ClientApplication& mApplication;

public:
    StockMarketGenerator(ClientApplication& app);
    void simulateMarket();
    
private:
    void generateOrder(TradeCore::OrderType tradeType);
};

