#include "TradeApp.h"
#include "Model.h"

TRADE_API_NAMESPACE_BEGIN

TradeApp::TradeApp(){}
TradeApp::~TradeApp(){
    for(auto& t: mThreadPool){
        t.join();
    }    
}
void TradeApp::run(){
    auto buy = [this]() {
        mTradeModel.generateTrade(TradeType::BUY);
    };
    auto sell = [this]() {
        mTradeModel.generateTrade(TradeType::SELL);
    };
    mThreadPool.emplace_back(buy);
    mThreadPool.emplace_back(sell);
}

TRADE_API_NAMESPACE_END