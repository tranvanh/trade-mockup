#include "TradeLib/OrderBook.h"
#include "TradeLib/StockMarket.h"
#include "TradeLib/Trade.h"
#include "UtilsLib/Logger.h"
#include "UtilsLib/TimePointUtils.h"
#include <iostream>

TRANVANH_NAMESPACE_BEGIN

OrderBook::OrderBook(StockMarket& stockMarket)
    : mStockMarket(stockMarket) {}

void OrderBook::registerOrder(const Order& order) {
    mOrderQueue.push(order);
}

void OrderBook::run() {
    auto& logger = Logger::instance();
    logger.log(Logger::LogLevel::DEBUG, "Initialize Order book");
    while (mStockMarket.isActive()) {
        const Order order = mOrderQueue.pop();
        switch (order.type) {
        case OrderType::BUY:
            processBuyer(order);
            break;
        case OrderType::SELL:
            processSeller(order);
            break;
        }
    }
}

OrderBook::~OrderBook() {}

void OrderBook::cleanUpBuyers(const std::unordered_set<int>& toRemove) {
    std::erase_if(mBuyers, [&toRemove](const std::pair<int, Order>& item) {
        return toRemove.contains(item.second.clientId);
    });
}

void OrderBook::cleanUpSellers(const std::unordered_set<int>& toRemove) {
    std::erase_if(mSellers, [&toRemove](const std::pair<int, Order>& item) {
        return toRemove.contains(item.second.clientId);
    });
}

void OrderBook::processBuyer(Order buyer) {
    std::unordered_set<int> toRemove;
    for (auto& [price, seller] : mSellers) {
        if (seller.price > buyer.price || buyer.volume <= 0) {
            break;
        }
        if (seller.clientId == buyer.clientId) {
            continue;
        }
        matchOrders(buyer, seller);
        if (seller.volume <= 0) {
            toRemove.insert(seller.clientId);
        }
    }
    if (buyer.volume > 0) {
        mBuyers.insert({ buyer.price, buyer });
    }
    cleanUpSellers(toRemove);
}

void OrderBook::processSeller(Order seller) {
        std::unordered_set<int> toRemove;
        for (auto& [price, buyer] : mBuyers) {
            if (seller.price > buyer.price || seller.volume <= 0) {
                break;
            }
            if (buyer.clientId == seller.clientId) {
                continue;
            }
            matchOrders(buyer, seller);
            if(buyer.volume <= 0){
                toRemove.insert(seller.clientId);
            }
        }
        if (seller.volume > 0) {
            mSellers.insert({ seller.price, seller });
        }
        cleanUpBuyers(toRemove);
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

TRANVANH_NAMESPACE_END
