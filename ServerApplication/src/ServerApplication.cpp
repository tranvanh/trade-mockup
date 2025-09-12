#include "ServerApplication.h"
#include <UtilsLib/Logger.h>
#include <TradeLib/Order.h>
#include <nlohmann/json.hpp>
#include <functional>
#include <vector>
#include <string>

ServerApplication::ServerApplication() : mServer(Server::AddressType::ANY) {
    registerCallback(mStockMarket.addOnTradeObserver([](const Trade& trade) {
        auto& logger = Logger::instance();
        logger.log(Logger::LogLevel::INFO, trade);
    }));
}

ServerApplication::~ServerApplication() {
    for (auto& t : mThreadPool) {
        t.join();
    }
}

void ServerApplication::run() {
    auto& logger = Logger::instance();
    logger.log(Logger::LogLevel::DEBUG, "Initialize application");
    isRunning = true;
    mStockMarket.run();
    mServer.openSocket();
    Logger::instance().setLevel(Logger::LogLevel::INFO);
    mServer.startListen(8080, [this](std::vector<char> bufferData, const int len){
          auto& logger = Logger::instance();
        logger.log(Logger::LogLevel::INFO, "Received message...");
        runBackgroundTask([this, bufferData, len]() {
            processServerMessage(std::string(bufferData.data(), len));
        });
       
    });
}

StockMarket& ServerApplication::getStockMarket() {
    return mStockMarket;
}
std::atomic<uint64_t> gIdCounterXXX = 0;

int randomValueOfMaxXXX(const int max) {
    return std::rand() / ((RAND_MAX + 1u) / max);
}
void ServerApplication::processServerMessage(const std::string& msg){
    auto& logger = Logger::instance();
    logger.log(Logger::LogLevel::INFO, "Processing server message...", msg);

    Order order;
    nlohmann::json msgJson = nlohmann::json::parse(msg);
    msgJson["id"].get_to(order.id);
    msgJson["price"].get_to(order.price);
    msgJson["volume"].get_to(order.volume);
    order.type = msgJson["type"].get<int>() == 0 ? OrderType::BUY : OrderType::SELL;
    order.timeStamp = std::chrono::system_clock::now();
    mStockMarket.registerOrder(order);
}
