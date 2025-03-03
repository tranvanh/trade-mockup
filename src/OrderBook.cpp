#include "OrderBook.h"
#include "Model.h"
#include "TradeApp.h"
#include <iostream>
#include <queue>

TRADE_API_NAMESPACE_BEGIN

template <typename Type>
void ThreadSafeQueue<Type>::push(const Type& value) {
    std::lock_guard<std::mutex> lock(m);
    mQueue.emplace_back(value);
    cv.notify_one();
}

template <typename Type>
Type ThreadSafeQueue<Type>::pop() {
    std::unique_lock<std::mutex> lock(m);
    cv.wait(lock, [this]() {
        return !mQueue.empty();
    });
    Type value = mQueue.front();
    mQueue.pop_front();
    return value;
}

OrderBook::OrderBook(TradeApp& app)
    : mApplication(app) {}

void OrderBook::registerOrder(const Order& order) {
    switch (order.type) {
    case OrderType::BUY:
        mBuyerQueue.push(order);
        break;
    case OrderType::SELL:
        mSellerQueue.push(order);
        break;
    }
}

void OrderBook::run() {
    mApplication.runBackgroundTask(std::bind_front(&OrderBook::processBuyers, this));
    mApplication.runBackgroundTask(std::bind_front(&OrderBook::processSellers, this));
    mApplication.runBackgroundTask(std::bind_front(&OrderBook::cleanUpBuyers, this));
    mApplication.runBackgroundTask(std::bind_front(&OrderBook::cleanUpSellers, this));
}

void OrderBook::cleanUpBuyers() {
    while (mApplication.isRunning) {
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(2000));
        std::unique_lock<std::mutex> buyersLock(mBuyers.lock);
        std::erase_if(mBuyers.data, [](const std::pair<int, Order>& item) {
            return item.second.volume <= 0;
        });
        buyersLock.unlock();
    }
}

void OrderBook::cleanUpSellers() {
    while (mApplication.isRunning) {
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(2000));
        std::unique_lock<std::mutex> sellersLock(mSellers.lock);
        std::erase_if(mSellers.data, [](const std::pair<int, Order>& item) {
            return item.second.volume <= 0;
        });
        sellersLock.unlock();
    }
}

void OrderBook::processBuyers() {
    while (mApplication.isRunning) {
        Order                        buyer = mBuyerQueue.pop();
        std::unique_lock<std::mutex> sellersLock(mSellers.lock);
        for (auto& [price, seller] : mSellers.data) {
            if (seller.volume <= 0) {
                continue;
            }
            if (buyer.price < seller.price) {
                break;
            }
            // todo correct the volume computation
            Trade trade{ .buyer     = buyer,
                         .seller    = seller,
                         .volume    = buyer.volume,
                         .tradeTime = std::chrono::system_clock::now() };
            mApplication.registerTrade(trade);
            const int sellerVolume = seller.volume;
            seller.volume -= buyer.volume;
            buyer.volume -= sellerVolume;
            if (buyer.volume <= 0) {
                break;
            }
        }
        sellersLock.unlock();
        if (buyer.volume > 0) {
            std::unique_lock<std::mutex> buyerLock(mBuyers.lock);
            mBuyers.data.insert({ buyer.price, buyer });
        }
    }
}

void OrderBook::processSellers() {
    while (mApplication.isRunning) {
        Order                        seller = mSellerQueue.pop();
        std::unique_lock<std::mutex> buyersLock(mBuyers.lock);
        for (auto& [price, buyer] : mBuyers.data) {
            if (buyer.volume <= 0) {
                continue;
            }
            if (seller.price > buyer.price) {
                break;
            }
            // todo correct the volume computation
            Trade trade{ .buyer     = buyer,
                         .seller    = seller,
                         .volume    = seller.volume,
                         .tradeTime = std::chrono::system_clock::now() };
            mApplication.registerTrade(trade);
            const int buyerVolume = buyer.volume;
            buyer.volume -= seller.volume;
            seller.volume -= buyerVolume;
            if (seller.volume <= 0) {
                break;
            }
        }
        buyersLock.unlock();
        if (seller.volume > 0) {
            std::unique_lock<std::mutex> buyerLock(mSellers.lock);
            mSellers.data.insert({ seller.price, seller });
        }
    }
}

TRADE_API_NAMESPACE_END
