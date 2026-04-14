#include "StockMarketGenerator.h"
#include <chrono>
#include <random>

namespace {
    constexpr int ID_COUNT = 100;

    int resolveClientId(const std::optional<int> fixedClientId, std::mt19937& generator) {
        if (fixedClientId) {
            return *fixedClientId;
        }

        std::uniform_int_distribution<int> idDistribution(1, ID_COUNT);
        return idDistribution(generator);
    }
}

StockMarketGenerator::StockMarketGenerator(std::function<void(TradeCore::Order)> registerOrder)
    : mRegisterOrder(std::move(registerOrder)) {}

StockMarketGenerator::~StockMarketGenerator() {
    stop();
}

void StockMarketGenerator::start(const std::optional<int> clientId) {
    stop();
    mActive = true;
    mWorker = std::thread([this, clientId]() { run(clientId); });
}

void StockMarketGenerator::stop() {
    mActive = false;
    if (mWorker.joinable()) {
        mWorker.join();
    }
}

void StockMarketGenerator::run(const std::optional<int> clientId) {
    std::random_device rd;
    std::mt19937       generator(rd());
    std::uniform_int_distribution<int> priceDistribution(1, TradeCore::PRICE_MAX);
    std::uniform_int_distribution<int> volumeDistribution(1, TradeCore::VOLUME_MAX);

    while (mActive) {
        for (int i = 0; i < 100 && mActive; ++i) {
            mRegisterOrder(TradeCore::Order(resolveClientId(clientId, generator),
                                            TradeCore::OrderType::BUY,
                                            priceDistribution(generator),
                                            volumeDistribution(generator)));
            if (!mActive) {
                break;
            }
            mRegisterOrder(TradeCore::Order(resolveClientId(clientId, generator),
                                            TradeCore::OrderType::SELL,
                                            priceDistribution(generator),
                                            volumeDistribution(generator)));
        }

        for (int i = 0; i < 20 && mActive; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}
