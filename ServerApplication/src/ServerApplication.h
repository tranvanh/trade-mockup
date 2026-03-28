#pragma once
#include <TradeCore/Market.h>
#include <TradeCore/Trade.h>
#include <Toybox/Application.h>
#include <Toybox/CallbackOwner.h>
#include <Toybox/Server.h>
#include <functional>
#include <optional>
#include <string>

class ServerApplication
    : public toybox::CallbackOwner
    , public toybox::Application {

    TradeCore::Market             mStockMarket;
    std::optional<toybox::Server> mServer;

    std::function<void(const TradeCore::Trade&)> mOnTrade;
    std::function<void(std::string)>             mOnEvent;

    enum class ROUTE {
        MARKET = 0,
        COMMAND = 1,
        SUBSCRIPTION = 2,
        SIZE = 3
    };

public:
    ServerApplication();

    // Interactive entry point: creates ServerCli locally, runs it, then cleans up.
    // ServerApplication is the outermost object; ServerCli lives and dies inside run().
    void run() override;
    void stop() override;

    // Called by ServerCli after config is gathered to start background threads
    void startBackground(int port, bool filled);

    void setOnTrade(std::function<void(const TradeCore::Trade&)> cb) { mOnTrade = std::move(cb); }
    void setOnEvent(std::function<void(std::string)> cb)             { mOnEvent = std::move(cb); }

private:
    void processServerMessage(const std::string& msg);
    void emitEvent(std::string msg);
};
