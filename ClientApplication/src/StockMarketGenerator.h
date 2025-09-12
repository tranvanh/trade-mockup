#pragma once
#include <TradeLib/Order.h>
#include <string>
#include <list>

using namespace tranvanh;

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

