
#include "Application.h"
#include <functional>

// APPLICATION_NAMESPACE_BEGIN

TradeApp::TradeApp() : mMarket(*this), mDatabase(*this), isRunning(false) {}
TradeApp::~TradeApp(){
    for(auto& t: mThreadPool){
        t.join();
    }
}

void TradeApp::run() {
    // 6 threads as result
    isRunning = true;
    mBook.run();
    mMarket.simulateMarket();
}

void TradeApp::registerOrder(const Order& order) {
    mBook.registerOrder(order);
}

void TradeApp::registerTrade(const Trade& trade) {
    mDatabase.registerTrade(trade);
}

void TradeApp::runBackgroundTask(const std::function<void()>& f){
    mThreadPool.emplace_back(f);
}

// APPLICATION_NAMESPACE_END