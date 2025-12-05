#include "TradeLib/StockMarket.h"
#include "TradeLib/Order.h"
#include "TradeLib/Trade.h"
#include "UtilsLib/TimePointUtils.h"
#include <chrono>
#include <gtest/gtest.h>
#include <sstream>

TRANVANH_NAMESPACE_BEGIN

TEST(StockMarket, IsInactiveByDefault) {
    StockMarket market;
    EXPECT_FALSE(market.isActive());
}

TEST(StockMarket, ObserverReceivesRegisteredTrade) {
    StockMarket market;

    int   callbackCount = 0;
    Trade* lastTrade = nullptr;
    auto  lifetime = market.addOnTradeObserver([&](const Trade& t) {
        ++callbackCount;
        EXPECT_EQ(lastTrade, nullptr);
        lastTrade = new Trade(t);
    });

    Order buyer{
        1, std::chrono::system_clock::now(), OrderType::BUY, 100, 10,
    };
    Order seller{
        2, std::chrono::system_clock::now(), OrderType::SELL, 100, 5,
    };

    Trade trade{
        seller,
        buyer,
        std::chrono::system_clock::now(),
        5,
    };

    market.registerTrade(trade);

    EXPECT_NE(lastTrade, nullptr);
    EXPECT_EQ(callbackCount, 1);
    EXPECT_EQ(lastTrade->volume, 5);
    EXPECT_EQ(lastTrade->buyer.clientId, 1);
    EXPECT_EQ(lastTrade->seller.clientId, 2);
    delete lastTrade;
}

TRANVANH_NAMESPACE_END
