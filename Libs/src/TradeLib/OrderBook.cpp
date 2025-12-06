#include "TradeLib/OrderBook.h"
#include "TradeLib/Trade.h"
#include "UtilsLib/Logger.h"


TRANVANH_NAMESPACE_BEGIN

OrderBook::~OrderBook() {
    mOrderQueue.stop();
}

void OrderBook::registerOrder(const Order& order) {
    mOrderQueue.push(order);
}

void OrderBook::pollOrders() {
    auto& logger = Logger::instance();
    logger.log(Logger::LogLevel::DEBUG, "Initialize Order book");
    const auto order = mOrderQueue.pop();
    ASSERT(order.has_value(), "Invalid order value");
    processOrder(*order);
}

void OrderBook::processOrder(const Order& order) {
    switch (order.type) {
    case OrderType::BUY:
        processBuyer(order);
        break;
    case OrderType::SELL:
        processSeller(order);
        break;
    }
}

void OrderBook::insertBuyer(const Order& buyer) {
    auto it = mBuyers.find(buyer.price);
    if (it == mBuyers.end()) {
        auto priceLevel    = std::make_shared<PriceLevel>();
        priceLevel->price  = buyer.price;
        priceLevel->volume = buyer.volume;
        priceLevel->orders.push_back(buyer);
        mBuyers.insert({ buyer.price, priceLevel });
    } else {
        auto priceLevel = it->second;
        priceLevel->volume += buyer.volume;
        priceLevel->orders.push_back(buyer);
    }
}

void OrderBook::insertSeller(const Order& seller) {
    auto it = mSellers.find(seller.price);
    if (it == mSellers.end()) {
        auto priceLevel    = std::make_shared<PriceLevel>();
        priceLevel->price  = seller.price;
        priceLevel->volume = seller.volume;
        priceLevel->orders.push_back(seller);
        mSellers.insert({ seller.price, priceLevel });
    } else {
        auto priceLevel = it->second;
        priceLevel->volume += seller.volume;
        priceLevel->orders.push_back(seller);
    }
}

void OrderBook::processBuyer(Order buyer) {
    int removeToRange = 0;
    for (auto& [price, level] : mSellers) {
        if (price > buyer.price || buyer.volume <= 0) {
            break;
        }
        matchOrders(buyer, *level);
        if (level->orders.empty()) {
            removeToRange = level->price;
        }
    }
    if (buyer.volume > 0) {
        insertBuyer(buyer);
    }
    const auto endRange = mSellers.find(removeToRange);
    mSellers.erase(mSellers.begin(), endRange);
}

void OrderBook::processSeller(Order seller) {
    int removeToRange = 0;
    for (auto& [price, level] : mBuyers) {
        if (seller.price > price || seller.volume <= 0) {
            break;
        }
        matchOrders(seller, *level);
        if (level->orders.empty()) {
            removeToRange = level->price;
        }
    }
    if (seller.volume > 0) {
        insertSeller(seller);
    }
    const auto endRange = mBuyers.find(removeToRange);
    mBuyers.erase(mBuyers.begin(), endRange);
}

int OrderBook::getSoldVolumes(const int buyer, const int seller) const {
    return buyer < seller ? buyer : seller;
}

void OrderBook::matchOrders(Order& requester, PriceLevel& level) {
    while (!level.orders.empty() && requester.volume > 0) {
        auto& order       = level.orders.front();
        const int   soldVolumes = getSoldVolumes(requester.volume, order.volume);
        const int   soldPrice   = requester.price < order.price ? requester.price : order.price;
        const int   buyerId     = requester.type == OrderType::BUY ? requester.clientId : order.clientId;
        const int   sellerId    = requester.type == OrderType::SELL ? requester.clientId : order.clientId;

        Trade trade(buyerId, sellerId, soldPrice, soldVolumes, std::chrono::system_clock::now());
        onTradeCallbacks(trade);

        requester.volume -= soldVolumes;
        order.volume -= soldVolumes;
        level.volume -= soldVolumes;

        if (order.volume <= 0) {
            level.orders.pop_front();
        }
    }
}

TRANVANH_NAMESPACE_END
