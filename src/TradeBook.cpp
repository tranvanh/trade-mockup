#include "TradeBook.h"
#include "Model.h"
#include "TradeApp.h"
#include <iostream>
#include <queue>

TRADE_API_NAMESPACE_BEGIN

template <typename Type>
void ThreadSafeQueue<Type>::push(const Type& value) {
    std::lock_guard<std::mutex> lock(m);
    mQueue.emplace_back(value);
    // std::cout << "PUSH " << value << std::endl;
    cv.notify_one();
}

template <typename Type>
Type ThreadSafeQueue<Type>::pop() {
    std::unique_lock<std::mutex> lock(m);
    cv.wait(lock, [this]() {
        return !mQueue.empty();
    });
    Type value = mQueue.front();
    // std::cout << "POP " << value << std::endl;
    mQueue.pop_front();
    return value;
}

TradeBook::TradeBook(TradeApp& app)
    : mApplication(app) {}

void TradeBook::registerTrade(const Trade& trade) {
    mQueue.push(trade);
}

void TradeBook::run() {
    mApplication.runBackgroundTask(std::bind_front(&TradeBook::processTrades, this));
    mApplication.runBackgroundTask(std::bind_front(&TradeBook::cleanUpBuyers, this));
    mApplication.runBackgroundTask(std::bind_front(&TradeBook::cleanUpSellers, this));
}

void TradeBook::cleanUpBuyers() {
    while (mApplication.isRunning) {
        std::cout << "CLEANING BUYERS" << std::endl;
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(2000));
        std::unique_lock<std::mutex> buyersLock(mBuyers.lock);

        for (const auto& item : mBuyers.data) {
            std::cout << item.second << std::endl;
        }
        std::cout << "--------------------------" << std::endl;

        std::erase_if(mBuyers.data, [](const std::pair<int, Trade>& item) {
            return item.second.volume <= 0;
        });

        for (const auto& item : mBuyers.data) {
            std::cout << item.second << std::endl;
        }
        std::cout << "FINISHED" << std::endl;
        buyersLock.unlock();
    }
}

void TradeBook::cleanUpSellers() {
    while (mApplication.isRunning) {
        // std::cout << "CLEANING SELLERS" << std::endl;
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(2000));
        std::unique_lock<std::mutex> sellersLock(mSellers.lock);
        // for (const auto& item : mSellers.data) {
        //     std::cout << item.second << std::endl;
        // }

        std::erase_if(mSellers.data, [](const std::pair<int, Trade>& item) {
            return item.second.volume <= 0;
        });

        // std::cout << "--------------------------" << std::endl;
        // for (const auto& item : mSellers.data) {
        //     std::cout << item.second << std::endl;
        // }
        // std::cout << "FINISHED" << std::endl;
        sellersLock.unlock();
    }
}

void TradeBook::processTrades() {
    while (mApplication.isRunning) {
        Trade trade = mQueue.pop();
        switch (trade.tradeType) {
        case TradeType::BUY:
            processBuyer(trade); // let it be own thread (use workers)
            break;
        case TradeType::SELL:
            processSeller(trade); // let it be own thread (use workers)
            break;
        }
    }
}

void TradeBook::processBuyer(Trade trade) {
    int                          volume = 0;
    std::unique_lock<std::mutex> buyersLock(mBuyers.lock);
    for (auto it = mSellers.data.lower_bound(trade.price); it != mSellers.data.end(); ++it) {
        Trade& seller = it->second;
        if (seller.volume <= 0) {
            continue;
        }
        if (trade.price < seller.price) {
            break;
        }
        // std::cout << "MATCH between: \n" << trade << "\n" << seller << std::endl;
        const int sellerVolume = seller.volume;
        seller.volume -= trade.volume;
        trade.volume -= sellerVolume;
        volume += sellerVolume;
        if (trade.volume <= 0) {
            break;
        }
    }


    buyersLock.unlock();
    // \todo improve
    if (volume > 0) {
        // std::cout << "--------------------------" << std::endl;
    }
    if (trade.volume > 0) {
        std::lock_guard<std::mutex> buyerLock(mBuyers.lock);
        mBuyers.data.insert({ trade.price, trade });
    }
}

void TradeBook::processSeller(Trade trade) {
    int                          volume = 0;
    std::unique_lock<std::mutex> sellersLock(mSellers.lock);
    for (auto it = mBuyers.data.lower_bound(trade.price); it != mBuyers.data.end(); ++it) {
        Trade& buyer = it->second;
        if (buyer.volume <= 0) {
            continue;
        }
        if (trade.price > buyer.price) {
            break;
        }
        // std::cout << "MATCH between: \n" << trade << "\n" << buyer << std::endl;
        const int buyerVolume = buyer.volume;
        buyer.volume -= trade.volume;
        trade.volume -= buyerVolume;
        volume += buyerVolume;
        if (trade.volume <= 0) {
            break;
        }
    }

    sellersLock.unlock();

    // \todo improve
    if (volume > 0) {
        // std::cout << "--------------------------" << std::endl;
    }


    if (trade.volume > 0) {
        std::lock_guard<std::mutex> buyerLock(mSellers.lock);
        mSellers.data.insert({ trade.price, trade });
    }
}

TRADE_API_NAMESPACE_END
