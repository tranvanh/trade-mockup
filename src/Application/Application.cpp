
#include "Application.h"
#include <functional>

// APPLICATION_NAMESPACE_BEGIN

TradeApp::TradeApp() : mGenerator(*this), isRunning(false) {
    registerCallback(mStockMarket.addOnTradeObserver([](const Trade& trade){
        std::cout << trade << std::endl;
    }));
}
TradeApp::~TradeApp(){
    for(auto& t: mThreadPool){
        t.join();
    }
}

void TradeApp::run() {
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