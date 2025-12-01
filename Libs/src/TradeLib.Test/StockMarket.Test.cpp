#include "TradeLib/StockMarket.h"
#include "TradeLib/Trade.h"
#include "TradeLib/Order.h"
#include "UtilsLib/TimePointUtils.h"
#include <gtest/gtest.h>
#include <chrono>
#include <sstream>

TRANVANH_NAMESPACE_BEGIN

TEST(StockMarket, IsInactiveByDefault) {
    StockMarket market;
    EXPECT_FALSE(market.isActive());
}

TEST(StockMarket, ObserverReceivesRegisteredTrade) {
    StockMarket market;

    int callbackCount = 0;
    Trade lastTrade{};
    auto lifetime = market.addOnTradeObserver([&](const Trade& t) {
        ++callbackCount;
        lastTrade = t;
    });

    Order buyer{
        .clientId = 1,
        .timeStamp = std::chrono::system_clock::now(),
        .type = OrderType::BUY,
        .price = 100,
        .volume = 10,
    };
    Order seller{
        .clientId = 2,
        .timeStamp = std::chrono::system_clock::now(),
        .type = OrderType::SELL,
        .price = 100,
        .volume = 5,
    };

    Trade trade{
        .seller = seller,
        .buyer = buyer,
        .tradeTime = std::chrono::system_clock::now(),
        .volume = 5,
    };

    market.registerTrade(trade);

    EXPECT_EQ(callbackCount, 1);
    EXPECT_EQ(lastTrade.volume, 5);
    EXPECT_EQ(lastTrade.buyer.clientId, 1);
    EXPECT_EQ(lastTrade.seller.clientId, 2);
}

TRANVANH_NAMESPACE_END
