#pragma once
#include "TradeLib/Order.h"
#include "TradeLib/Trade.h"
#include "UtilsLib/ThreadSafeQueue.h"
#include <map>
#include <unordered_set>
#include "UtilsLib/CallbackList.h"


TRANVANH_NAMESPACE_BEGIN

class Market;

/// We receive orders through one serialized queue. Order then gets processed, the order book is always in
/// valid state. All is done on main thread, but note that registerOrder can be called from different threads
class OrderBook {
    struct PriceLevel {
        int price = 0;
        int volume = 0;
        std::deque<Order> orders;
    };

    ThreadSafeQueue<Order>                       mOrderQueue;
    std::map<int, std::shared_ptr<PriceLevel>, std::greater<>> mBuyers;
    std::map<int, std::shared_ptr<PriceLevel>>                    mSellers;

public:
    void registerOrder(const Order& order);
    void processOrders();

    CallbackList<void(const Trade& trade)> onTradeCallbacks;

private:

    void processOrder(const Order& buyer);
    void insertBuyer(const Order& buyer);
    void insertSeller(const Order& seller);
    void processBuyer(Order buyer);
    void processSeller(Order seller);

    // void cleanUpBuyers(const std::unordered_set<int>& toRemove);
    // void cleanUpSellers(const std::unordered_set<int>& toRemove);

    int getSoldVolumes(const int buyer, const int seller) const;

    // Make a trade and update volumes
    void matchOrders(Order& buyer, PriceLevel& seller);
};

TRANVANH_NAMESPACE_END
