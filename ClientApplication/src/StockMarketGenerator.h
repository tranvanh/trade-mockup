#pragma once
#include <TradeCore/Order.h>
#include <atomic>

class ClientApplication;

class StockMarketGenerator {
    ClientApplication&   mApplication;
    std::atomic_bool     mSimActive{false};

public:
    explicit StockMarketGenerator(ClientApplication& app);

    void start();
    void stop();
    bool isActive() const { return mSimActive.load(); }

private:
    void simulateMarket();
    void generateOrder(TradeCore::OrderType type);
};
