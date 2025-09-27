#pragma once
#include "StockMarketGenerator.h"
#include <TradeLib/Order.h>
#include <unordered_map>
#include <UtilsLib/Application.h>
#include <UtilsLib/Client.h>

using namespace tranvanh;

constexpr int THREAD_COUNT = 2;

class ClientApplication : public Application {
    uint mId = 0;
    enum CommandType { BUY, SELL, EXIT, INVALID };
    struct Command {
        CommandType type   = INVALID;
        int         price  = 0;
        int         volume = 0;
    };

    bool                 mSimulation = false;
    StockMarketGenerator mGenerator;
    Client               mClient;

public:
    ClientApplication(const uint id, const bool isSimulation = false)
        : Application(THREAD_COUNT)
        , mId(id)
        , mSimulation(isSimulation)
        , mGenerator(*this){};
    virtual void run() override;
    void         registerOrder(Order order) const;

private:
    void    processUserInputs() const;
    Command parseCommand(const std::string& line) const;
    void    handleCommand(const Command& cmd) const;
};