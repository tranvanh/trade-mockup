#include "TradeCommon/BoundedHistory.h"
#include "TradeCommon/ClientOptions.h"
#include "TradeCommon/Protocol.h"
#include "TradeCommon/ServerOptions.h"
#include "TradeCommon/Validation.h"
#include <gtest/gtest.h>

TEST(ClientOptions, UsesTuiModeByDefault) {
    const auto parsed = TradeCommon::parseClientLaunchOptions({"client"});
    EXPECT_FALSE(parsed.showHelp);
    EXPECT_EQ(parsed.options.mode, TradeCommon::ClientLaunchMode::Tui);
}

TEST(ClientOptions, UsesLegacyManualWhenIdProvided) {
    const auto parsed = TradeCommon::parseClientLaunchOptions({"client", "--id=7"});
    EXPECT_EQ(parsed.options.mode, TradeCommon::ClientLaunchMode::LegacyManual);
    EXPECT_EQ(parsed.options.clientId, 7);
}

TEST(ClientOptions, UsesLegacySimulationWhenRequested) {
    const auto parsed = TradeCommon::parseClientLaunchOptions({"client", "--simulate"});
    EXPECT_EQ(parsed.options.mode, TradeCommon::ClientLaunchMode::LegacySimulation);
}

TEST(ClientOptions, RejectsInvalidCombination) {
    EXPECT_THROW((void)TradeCommon::parseClientLaunchOptions({"client", "--id=5", "--simulate"}),
                 std::invalid_argument);
}

TEST(ServerOptions, UsesFilledFlag) {
    const auto parsed = TradeCommon::parseServerLaunchOptions({"server", "--filled"});
    EXPECT_TRUE(parsed.options.filled);
}

TEST(Protocol, RoundTripsSubmitOrderEnvelope) {
    const auto envelope = TradeCommon::makeSubmitOrderEnvelope(9, TradeCore::OrderType::SELL, 42, 11);
    const auto parsed   = TradeCommon::parseEnvelope(TradeCommon::serializeEnvelope(envelope));

    EXPECT_EQ(parsed.type, TradeCommon::MessageType::SubmitOrder);
    const auto submit = std::get<TradeCommon::SubmitOrderPayload>(parsed.payload);
    EXPECT_EQ(submit.clientId, 9);
    EXPECT_EQ(submit.side, TradeCore::OrderType::SELL);
    EXPECT_EQ(submit.price, 42);
    EXPECT_EQ(submit.volume, 11);
}

TEST(Protocol, RoundTripsTradeEnvelope) {
    const TradeCore::Trade trade(10, 20, 120, 3);
    const auto             parsed = TradeCommon::parseEnvelope(
        TradeCommon::serializeEnvelope(TradeCommon::makeTradeEnvelope(trade)));

    EXPECT_EQ(parsed.type, TradeCommon::MessageType::Trade);
    const auto tradePayload = std::get<TradeCommon::TradePayload>(parsed.payload);
    EXPECT_EQ(tradePayload.sellerId, 10);
    EXPECT_EQ(tradePayload.buyerId, 20);
    EXPECT_EQ(tradePayload.price, 120);
    EXPECT_EQ(tradePayload.volume, 3);
    EXPECT_FALSE(tradePayload.tradeTime.empty());
}

TEST(BoundedHistory, TrimsToConfiguredCapacity) {
    TradeCommon::BoundedHistory<int> history(3);
    history.push(1);
    history.push(2);
    history.push(3);
    history.push(4);

    const auto snapshot = history.snapshot();
    ASSERT_EQ(snapshot.size(), 3u);
    EXPECT_EQ(snapshot[0], 2);
    EXPECT_EQ(snapshot[1], 3);
    EXPECT_EQ(snapshot[2], 4);
}

TEST(Validation, RejectsInvalidClientId) {
    const auto error = TradeCommon::validateClientId(0);
    ASSERT_TRUE(error.has_value());
    EXPECT_FALSE(error->empty());
}

TEST(Validation, RejectsInvalidOrderValues) {
    const auto error = TradeCommon::validateOrder(10, 0);
    ASSERT_TRUE(error.has_value());
    EXPECT_FALSE(error->empty());
}
