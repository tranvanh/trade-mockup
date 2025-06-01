
#include "TradeApp.h"
#include "Logger.h"
#include <functional>

// APPLICATION_NAMESPACE_BEGIN

TradeApp::TradeApp()
    : mGenerator(*this)
    , mInvestor(mStockMarket) {
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
    mGenerator.simulateMarket();
}

void TradeApp::runBackgroundTask(const std::function<void()>& f){
    mThreadPool.emplace_front(f);
}

StockMarket& TradeApp::getStockMarket() {
    return mStockMarket;
}

// APPLICATION_NAMESPACE_END