#pragma once
#include <TradeCore/Order.h>
#include <atomic>
#include <functional>
#include <optional>
#include <thread>

class StockMarketGenerator {
    std::function<void(TradeCore::Order)> mRegisterOrder;
    std::atomic_bool                      mActive = false;
    std::thread                           mWorker;

public:
    explicit StockMarketGenerator(std::function<void(TradeCore::Order)> registerOrder);
    ~StockMarketGenerator();

    void start(std::optional<int> clientId = std::nullopt);
    void stop();
    [[nodiscard]] bool isActive() const { return mActive.load(); }

private:
    void run(std::optional<int> clientId);
};
