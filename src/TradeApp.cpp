
#include <functional>
#include "TradeApp.h"
#include "Model.h"
#include "OrderBook.h"

TRADE_API_NAMESPACE_BEGIN

TradeApp::TradeApp() : mModel(*this), mBook(*this), mDatabase(*this) {}
TradeApp::~TradeApp(){
    for(auto& t: mThreadPool){
        t.join();
    }
}

void TradeApp::run() {
    // 6 threads as result
    isRunning = true;
    mBook.run();
    mModel.simulateMarket();
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

TRADE_API_NAMESPACE_END