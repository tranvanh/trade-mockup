#include "ServerApplication.h"
#include <functional>
#include <nlohmann/json.hpp>
#include <random>
#include <string>
#include <TradeLib/Order.h>
#include <UtilsLib/Logger.h>
#include <vector>

constexpr int THREAD_COUNT = 6;
constexpr int FILLED_COUNT = 100;

void fillMarketWithMockData(Market& market, int count) {
    if (count <= 0)
        return;

    std::random_device rd;
    std::mt19937       gen(rd());

    std::uniform_int_distribution<int> typeDist(0, 1);
    std::uniform_int_distribution<int> priceDist(1, PRICE_MAX);
    std::uniform_int_distribution<int> volumeDist(1, VOLUME_MAX);
    std::uniform_int_distribution<int> clientDist(1, 1'000'000);

    for (int i = 0; i < count; ++i) {
        const auto type   = typeDist(gen) == 0 ? OrderType::BUY : OrderType::SELL;
        const int  price  = priceDist(gen);
        const int  volume = volumeDist(gen);
        const int  client = clientDist(gen);

        Order order(client, type, price, volume);
        market.registerOrder(order);
    }
}

ServerApplication::ServerApplication(const bool filled)
    : Application(THREAD_COUNT)
    , mServer(Server::AddressType::ANY) {
    auto& logger = Logger::instance();
    if (filled) {
        logger.log(Logger::LogLevel::INFO, "Filling the market...");
        fillMarketWithMockData(mStockMarket, FILLED_COUNT);
        logger.log(Logger::LogLevel::INFO, "Market filled with ", FILLED_COUNT, " orders.");
    }

    registerCallback(mStockMarket.addOnTradeObserver([&logger](const Trade& trade) {
        logger.log(Logger::LogLevel::INFO, trade);
    }));
}

void ServerApplication::run() {
    Application::run();
    auto& logger = Logger::instance();
    logger.log(Logger::LogLevel::DEBUG, "Initialize application");
    runBackgroundTask([this]() {
        mStockMarket.run();
    });
    auto onReceive = [this](std::vector<char> bufferData, const int len) {
        auto& logger = Logger::instance();
        logger.log(Logger::LogLevel::INFO, "Received message...");
        runBackgroundTask([this, bufferData, len]() {
            processServerMessage(std::string(bufferData.data(), len));
        });
    };
    if (!mServer.openSocket() || mServer.startListen(8080, onReceive)) {
        stop();
    }
}

void ServerApplication::processServerMessage(const std::string& msg) {
    auto& logger = Logger::instance();
    logger.log(Logger::LogLevel::INFO, "Processing server message...", msg);

    int            clientId = 0;
    int            price    = 0;
    int            volume   = 0;
    OrderType      type     = OrderType::BUY;
    nlohmann::json msgJson  = nlohmann::json::parse(msg);
    msgJson["clientId"].get_to(clientId);
    msgJson["price"].get_to(price);
    msgJson["volume"].get_to(volume);
    type = msgJson["type"].get<int>() == 0 ? OrderType::BUY : OrderType::SELL;
    Order order(clientId, type, price, volume);
    mStockMarket.registerOrder(order);
}
