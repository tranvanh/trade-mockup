#include "OrderBook.h"
// #include "Application.h"
#include "Trade.h"
#include <iostream>
#include <queue>

// TRADE_NAMESPACE_BEGIN

// OrderBook::OrderBook(TradeApp& app)
//     : mApplication(app) {}

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
    // mApplication.runBackgroundTask(std::bind_front(&OrderBook::processBuyers, this));
    // mApplication.runBackgroundTask(std::bind_front(&OrderBook::processSellers, this));
    // mApplication.runBackgroundTask(std::bind_front(&OrderBook::cleanUpBuyers, this));
    // mApplication.runBackgroundTask(std::bind_front(&OrderBook::cleanUpSellers, this));
}

void OrderBook::cleanUpBuyers() {
    // while (mApplication.isRunning) {
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(5000));
        std::unique_lock<std::mutex> buyersLock(mBuyers.lock);
        std::erase_if(mBuyers.data, [](const std::pair<int, Order>& item) {
            return item.second.volume <= 0;
        });
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(2000)); // heavy load actor
        buyersLock.unlock();
    // }
}

void OrderBook::cleanUpSellers() {
    // while (mApplication.isRunning) {
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(5000));
        std::unique_lock<std::mutex> sellersLock(mSellers.lock);
        std::erase_if(mSellers.data, [](const std::pair<int, Order>& item) {
            return item.second.volume <= 0;
        });
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(2000)); // heavy load actor
        sellersLock.unlock();
    // }
}

void OrderBook::processBuyers() {
    // while (mApplication.isRunning) {
        Order                        buyer = mBuyerQueue.pop();
        std::unique_lock<std::mutex> sellersLock(mSellers.lock);
        for (auto& [price, seller] : mSellers.data) {
            if (seller.price > buyer.price || buyer.volume <= 0) {
                break;
            }
            if (seller.volume <= 0) {
                continue;
            }
            matchOrders(buyer, seller);
        }
        sellersLock.unlock();
        if (buyer.volume > 0) {
            std::unique_lock<std::mutex> buyerLock(mBuyers.lock);
            mBuyers.data.insert({ buyer.price, buyer });
        }
    // }
}

void OrderBook::processSellers() {
    // while (mApplication.isRunning) {
        Order                        seller = mSellerQueue.pop();
        std::unique_lock<std::mutex> buyersLock(mBuyers.lock);
        for (auto& [price, buyer] : mBuyers.data) {
            if (seller.price > buyer.price || seller.volume <= 0) {
                break;
            }
            if (buyer.volume <= 0) {
                continue;
            }
            matchOrders(buyer, seller);
        }
        buyersLock.unlock();
        if (seller.volume > 0) {
            std::unique_lock<std::mutex> buyerLock(mSellers.lock);
            mSellers.data.insert({ seller.price, seller });
        }
    // }
}

int OrderBook::getSoldVolumes(const int buyer, const int seller) const {
    return buyer < seller ? buyer : seller;
}

void OrderBook::matchOrders(Order& buyer, Order& seller) {
    Trade trade{
        .seller    = seller,
        .buyer     = buyer,
        .tradeTime = std::chrono::system_clock::now(),
        .volume    = getSoldVolumes(seller.volume, buyer.volume),
    };
    // mApplication.registerTrade(trade);
    buyer.volume -= seller.volume;
    seller.volume -= trade.volume;
}

// TRADE_NAMESPACE_END
