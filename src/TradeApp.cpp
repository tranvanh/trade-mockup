
#include <functional>
#include "TradeApp.h"
#include "Model.h"
#include "TradeDatabase.h"

TRADE_API_NAMESPACE_BEGIN

TradeApp::TradeApp() : mModel(*this), mDatabase(*this){}
TradeApp::~TradeApp(){
    for(auto& t: mThreadPool){
        t.join();
    }    
}

void TradeApp::run(){
    runBackgroundTask(std::bind_front(&TradeDatabase::run, &mDatabase));
    mModel.simulate();
}

void TradeApp::registerTrade(const Trade& trade){
    mDatabase.registerTrade(trade);
}

void TradeApp::runBackgroundTask(const std::function<void()>& f){
    mThreadPool.emplace_back(f);
}

TRADE_API_NAMESPACE_END