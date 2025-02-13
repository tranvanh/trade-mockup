#include "TradeDatabase.h"
#include "TradeApp.h"
#include "Model.h"
#include <iostream>

TRADE_API_NAMESPACE_BEGIN

template<typename Type>
void ThreadSafeQueue<Type>::push(const Type& value){
    std::lock_guard<std::mutex> lock(m);
    mQueue.emplace_back(value);
    std::cout << "PUSH " << value << std::endl;
    cv.notify_one();
}

template<typename Type>
Type ThreadSafeQueue<Type>::pop(){
    std::unique_lock<std::mutex> lock(m);
    cv.wait(lock, [this](){return !mQueue.empty();});
    Type value = mQueue.front();
    std::cout << "POP " << value << std::endl;
    mQueue.pop_front();
    return value;
}


TradeDatabase::TradeDatabase(TradeApp& app) : mApplication(app){}

void TradeDatabase::registerTrade(const Trade& trade){
    mQueue.push(trade);
}

void TradeDatabase::run(){
    const auto processTrades = [this](){
        while(!mQueue.empty()){
            Trade trade = mQueue.pop();
            mData.insert({trade.id, trade});
        }
    };

    while(true){
        processTrades();
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(1000));
    }
}

TRADE_API_NAMESPACE_END
