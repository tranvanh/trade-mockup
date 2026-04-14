#pragma once
#include <TradeCommon/BoundedHistory.h>
#include <TradeCommon/ServerOptions.h>
#include <TradeCommon/TcpServer.h>
#include <TradeCore/Market.h>
#include <Toybox/Application.h>
#include <Toybox/CallbackOwner.h>
#include <cstdint>
#include <functional>
#include <mutex>
#include <string>
#include <vector>

class ServerApplication final
    : public toybox::CallbackOwner
    , public toybox::Application {
    static constexpr int THREAD_COUNT = 6;

    TradeCommon::ServerLaunchOptions mOptions;
    TradeCore::Market                mStockMarket;
    TradeCommon::TcpServer           mServer;
    TradeCommon::BoundedHistory<std::string> mConnectionHistory;
    TradeCommon::BoundedHistory<std::string> mTradeHistory;
    mutable std::mutex               mNotifierMutex;
    std::function<void()>            mUiNotifier;

public:
    explicit ServerApplication(TradeCommon::ServerLaunchOptions options);
    void run() override;

private:
    void                         runTui();
    void                         processServerMessage(std::uint64_t connectionId, const std::string& msg);
    void                         pushConnectionEvent(std::string message);
    void                         pushTradeEvent(std::string message);
    void                         notifyUi() const;
    [[nodiscard]] std::vector<std::string> connectionSnapshot() const;
    [[nodiscard]] std::vector<std::string> tradeSnapshot() const;
};
