#pragma once
#include "StockMarketGenerator.h"
#include <TradeLib/Order.h>
#include <unordered_map>
#include <UtilsLib/Application.h>
#include <UtilsLib/Client.h>

using namespace tranvanh;

class ClientApplication : public Application {
    uint mId = 0;
    enum Command { BUY, SELL, EXIT };
    bool                 mSimulation = false;
    StockMarketGenerator mGenerator;

    Client                                         mClient;
    const std::unordered_map<std::string, Command> mCommands = {
        { "buy", BUY },
        { "sell", SELL },
        { "exit", EXIT },
    };

public:
    ClientApplication(const uint id, const bool isSimulation = false)
        : Application(2), mId(id)
        , mSimulation(isSimulation)
        , mGenerator(*this){};
    virtual void run() override;
    void         registerOrder(Order order) const;

private:
    void processUserInputs() const;
    void proccessOrderInput(Order& order) const;
};