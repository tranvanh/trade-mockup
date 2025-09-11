#include "TradeApp.h"
#include "UtilsLib/Logger.h"
#include "TradeLib/Order.h"
#include <functional>
#include <vector>
// #include <memory>

// APPLICATION_NAMESPACE_BEGIN

TradeApp::TradeApp()
    : mGenerator(*this)
    , mInvestor(mStockMarket), mServer(Server::AddressType::ANY) {
    registerCallback(mStockMarket.addOnTradeObserver([](const Trade& trade) {
        auto& logger = Logger::instance();
        logger.log(Logger::LogLevel::INFO, trade);
    }));
}

TradeApp::~TradeApp() {
    for (auto& t : mThreadPool) {
        t.join();
    }
}

void TradeApp::run() {
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
    // mGenerator.simulateMarket();
}

void TradeApp::runBackgroundTask(const std::function<void()>& f) {
    mThreadPool.emplace_front(f);
}

StockMarket& TradeApp::getStockMarket() {
    return mStockMarket;
}
std::atomic<uint64_t> gIdCounterXXX = 0;

int randomValueOfMaxXXX(const int max) {
    return std::rand() / ((RAND_MAX + 1u) / max);
}
void TradeApp::processServerMessage(const std::string& msg){
    auto& logger = Logger::instance();
    logger.log(Logger::LogLevel::INFO, "Processing server message...", msg);
    Order order;
    order.id = gIdCounterXXX++;
    order.price = 1;
    order.timeStamp = std::chrono::system_clock::now();
    order.type = msg == "buy" ? OrderType::BUY : OrderType::SELL;
    order.volume = 1;
    mStockMarket.registerOrder(order);
}

// APPLICATION_NAMESPACE_END