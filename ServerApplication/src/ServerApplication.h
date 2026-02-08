#pragma once
#include <atomic>
#include <forward_list>
#include <functional>
#include <thread>
#include <TradeLib/Order.h>
#include <unordered_map>
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

    struct Address {
        int socket = -1;
        int port = -1;
    };

    enum class ROUTE {
        MARKET = 0,
        COMMAND = 1,
        SUBSCRIPTION = 2,
        SIZE = 3
    };

    std::vector<Address>    mAddressBook;

public:
    ServerApplication(const bool filled = false);
    virtual void run() override;

private:
    void processServerMessage(const std::string& msg);
};

