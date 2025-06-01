#include "OrderBook.h"
#include "Logger.h"
#include "StockMarket.h"
#include "TimePointUtils.h"
#include "Trade.h"
#include <iostream>
#include <queue>

// TRADE_NAMESPACE_BEGIN

OrderBook::OrderBook(StockMarket& stockMarket)
    : mStockMarket(stockMarket) {}

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
    auto& logger = Logger::instance();
    logger.log(Logger::LogLevel::DEBUG, "Initialize Order book");

    mThreadPool.emplace_front(std::bind_front(&OrderBook::processBuyers, this));
    mThreadPool.emplace_front(std::bind_front(&OrderBook::processSellers, this));
    mThreadPool.emplace_front(std::bind_front(&OrderBook::cleanUpBuyers, this));
    mThreadPool.emplace_front(std::bind_front(&OrderBook::cleanUpSellers, this));
}

OrderBook::~OrderBook() {
    for (auto& t : mThreadPool) {
        t.join();
    }
}

void OrderBook::cleanUpBuyers() {
    auto& logger = Logger::instance();
    while (mStockMarket.isActive()) {

        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(5000));
        std::unique_lock<std::mutex> buyersLock(mBuyers.lock);
        logger.log(Logger::LogLevel::DEBUG, "Start cleaning invalid buyers...");
        std::erase_if(mBuyers.data, [](const std::pair<int, Order>& item) {
            return item.second.volume <= 0;
        });
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(3000)); // heavy load actor
        logger.log(Logger::LogLevel::DEBUG, "Finish cleaning invalid buyers...");
        buyersLock.unlock();
    }
}

void OrderBook::cleanUpSellers() {
    auto& logger = Logger::instance();
    while (mStockMarket.isActive()) {
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(5000));
        std::unique_lock<std::mutex> sellersLock(mSellers.lock);
        logger.log(Logger::LogLevel::DEBUG, "Start cleaning invalid sellers...");
        std::erase_if(mSellers.data, [](const std::pair<int, Order>& item) {
            return item.second.volume <= 0;
        });
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(2000)); // heavy load actor
        logger.log(Logger::LogLevel::DEBUG, "Finish cleaning invalid sellers...");
        sellersLock.unlock();
    }
}

void OrderBook::processBuyers() {
    while (mStockMarket.isActive()) {
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
    }
}

void OrderBook::processSellers() {
    while (mStockMarket.isActive()) {
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
    }
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
    mStockMarket.registerTrade(trade);
    buyer.volume -= seller.volume;
    seller.volume -= trade.volume;
}

// TRADE_NAMESPACE_END
