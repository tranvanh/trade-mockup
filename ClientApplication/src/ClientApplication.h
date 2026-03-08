#pragma once
#include "StockMarketGenerator.h"
#include <TradeCore/Order.h>
#include <Toybox/Application.h>
#include <Toybox/Client.h>

constexpr int THREAD_COUNT = 2;

class ClientApplication : public toybox::Application {
    uint mId = 0;
    enum CommandType { BUY, SELL, EXIT, INVALID };
    struct Command {
        CommandType type   = INVALID;
        int         price  = 0;
        int         volume = 0;
    };

    bool                 mSimulation = false;
    StockMarketGenerator mGenerator;
    toybox::Client               mClient;

public:
    explicit ClientApplication(const uint id, const bool isSimulation = false)
        : toybox::Application(THREAD_COUNT)
        , mId(id)
        , mSimulation(isSimulation)
        , mGenerator(*this){};
    virtual void run() override;
    void         registerOrder(TradeCore::Order order) const;

private:
    void    processUserInputs() const;
    Command parseCommand(const std::string& line) const;
    void    handleCommand(const Command& cmd) const;
};