#pragma once
#include <atomic>
#include <forward_list>
#include <functional>
#include <thread>
#include <TradeLib/Order.h>
#include <TradeLib/StockMarket.h>
#include <TradeLib/Trade.h>
#include <UtilsLib/Application.h>
#include <UtilsLib/CallbackOwner.h>
#include <UtilsLib/Server.h>

using namespace tranvanh;

class ServerApplication
    : public CallbackOwner
    , public Application {
    StockMarket                    mStockMarket;
    Server                         mServer;

public:
    ServerApplication();

    virtual void run() override;
    StockMarket& getStockMarket();

private:
    void processServerMessage(const std::string& msg);
};

