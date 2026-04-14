#pragma once
#include "StockMarketGenerator.h"
#include <TradeCommon/BoundedHistory.h>
#include <TradeCommon/ClientOptions.h>
#include <TradeCommon/Protocol.h>
#include <TradeCommon/TcpClient.h>
#include <TradeCore/Order.h>
#include <Toybox/Application.h>
#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

class ClientApplication final : public toybox::Application {
    static constexpr int THREAD_COUNT = 2;

    enum class CommandType { BUY, SELL, EXIT, INVALID };

    struct Command {
        CommandType type   = CommandType::INVALID;
        int         price  = 0;
        int         volume = 0;
    };

    TradeCommon::ClientLaunchOptions  mOptions;
    mutable std::mutex                mClientMutex;
    std::unique_ptr<TradeCommon::TcpClient> mClient;
    StockMarketGenerator mGenerator;
    TradeCommon::BoundedHistory<std::string> mTradeHistory;
    TradeCommon::BoundedHistory<std::string> mStatusHistory;
    mutable std::mutex                mNotifierMutex;
    std::function<void()>             mUiNotifier;
    std::atomic_bool                  mConnected = false;

public:
    explicit ClientApplication(TradeCommon::ClientLaunchOptions options)
        : toybox::Application(THREAD_COUNT)
        , mOptions(std::move(options))
        , mGenerator([this](TradeCore::Order order) { registerOrder(std::move(order)); })
        , mTradeHistory(100)
        , mStatusHistory(100) {}

    void run() override;
    void registerOrder(TradeCore::Order order);

private:
    bool                    connectToServer(const std::string& host, unsigned short port);
    void                    disconnectFromServer(const std::string& reason = {});
    void                    handleServerMessage(const std::string& message);
    void                    runLegacyManual();
    void                    runLegacySimulation();
    void                    runTui();
    void                    processUserInputs(int clientId);
    void                    pushStatus(std::string message);
    void                    pushTrade(std::string message);
    void                    notifyUi() const;
    bool                    sendEnvelope(const TradeCommon::Envelope& envelope);
    bool                    submitOrder(int clientId, TradeCore::OrderType side, int price, int volume);
    [[nodiscard]] std::vector<std::string> statusSnapshot() const;
    [[nodiscard]] std::vector<std::string> tradeSnapshot() const;
    [[nodiscard]] Command    parseCommand(const std::string& line) const;
};
