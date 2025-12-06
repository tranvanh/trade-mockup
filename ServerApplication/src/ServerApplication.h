#pragma once
#include <atomic>
#include <forward_list>
#include <functional>
#include <thread>
#include <TradeLib/Order.h>
#include <TradeLib/Market.h>
#include <TradeLib/Trade.h>
#include <UtilsLib/Application.h>
#include <UtilsLib/CallbackOwner.h>
#include <UtilsLib/Server.h>

using namespace tranvanh;

class ServerApplication
    : public CallbackOwner
    , public Application {
    Market                    mStockMarket;
    Server                         mServer;

public:
    ServerApplication();
    virtual void run() override;

private:
    void processServerMessage(const std::string& msg);
};

