
#include "Application.h"
#include <functional>

// APPLICATION_NAMESPACE_BEGIN

TradeApp::TradeApp() : mGenerator(*this), isRunning(false) {}
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
    mThreadPool.emplace_back(f);
}

// APPLICATION_NAMESPACE_END