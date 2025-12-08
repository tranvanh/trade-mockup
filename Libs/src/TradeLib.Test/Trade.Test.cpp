#include "TradeLib/Trade.h"
#include "TradeLib/Market.h"
#include "TradeLib/Order.h"
#include <chrono>
#include <gtest/gtest.h>
#include <sstream>

TRANVANH_NAMESPACE_BEGIN


TEST(Trade, OstreamOutputsKeyFields) {
    Order buyer( 123, OrderType::BUY, 150, 3 );
    Order seller( 456, OrderType::SELL, 150, 3 );
    Trade trade(seller.clientId, buyer.clientId, 100, 3);

    std::ostringstream oss;
    oss << trade;
    const auto s = oss.str();

    // Expect the formatted output to contain buyer/seller IDs and volume
    EXPECT_NE(s.find("buyer="), std::string::npos);
    EXPECT_NE(s.find("seller="), std::string::npos);
    EXPECT_NE(s.find("price="), std::string::npos);
    EXPECT_NE(s.find("volume="), std::string::npos);
}

TRANVANH_NAMESPACE_END
