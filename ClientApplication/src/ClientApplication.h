#pragma once
#include "ClientCli.h"
#include "StockMarketGenerator.h"
#include <TradeCore/Order.h>
#include <Toybox/Application.h>
#include <Toybox/Client.h>
#include <string>

constexpr int THREAD_COUNT = 2;

class ClientApplication : public toybox::Application {
    uint mId{0};
    std::string mServer;
    int mPort{0};
    StockMarketGenerator mGenerator;
    toybox::Client mClient;
    ClientCli mCli;

public:
    ClientApplication();

    // Interactive entry point: runs the CLI (prompts config, connects, command loop)
    void run() override;

    // Called by Cli after config is gathered
    void configure(int id, std::string server, int port);

    bool connect();

    void simulateMarket();

    void registerOrder(TradeCore::Order order) const;

    int id() const { return static_cast<int>(mId); }
};
