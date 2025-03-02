
#include <functional>
#include "TradeApp.h"
#include "Model.h"
#include "TradeBook.h"

TRADE_API_NAMESPACE_BEGIN

TradeApp::TradeApp() : mModel(*this), mBook(*this){}
TradeApp::~TradeApp(){
    for(auto& t: mThreadPool){
        t.join();
    }
}

void TradeApp::run() {
    // 3 threads as result
    isRunning = true;
    mBook.run();
    mModel.simulate();
}

void TradeApp::registerTrade(const Trade& trade) {
    mBook.registerTrade(trade);
}

void TradeApp::runBackgroundTask(const std::function<void()>& f){
    mThreadPool.emplace_back(f);
}

TRADE_API_NAMESPACE_END