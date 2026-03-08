#pragma once
#include <TradeCore/Market.h>
#include <Toybox/Application.h>
#include <Toybox/CallbackOwner.h>
#include <Toybox/Server.h>

class ServerApplication
    : public toybox::CallbackOwner
    , public toybox::Application {
    TradeCore::Market                    mStockMarket;
    toybox::Server                         mServer;

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

