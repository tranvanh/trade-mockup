#pragma once
#include <TradeCore/Order.h>
#include <TradeCore/Trade.h>
#include <string>
#include <string_view>
#include <variant>

namespace TradeCommon {

    enum class MessageType { SubmitOrder, Trade, Error };

    struct SubmitOrderPayload {
        int                  clientId = 0;
        TradeCore::OrderType side     = TradeCore::OrderType::BUY;
        int                  price    = 0;
        int                  volume   = 0;
    };

    struct TradePayload {
        int         sellerId  = 0;
        int         buyerId   = 0;
        int         price     = 0;
        int         volume    = 0;
        std::string tradeTime;
    };

    struct ErrorPayload {
        std::string message;
    };

    using MessagePayload = std::variant<SubmitOrderPayload, TradePayload, ErrorPayload>;

    struct Envelope {
        MessageType    type    = MessageType::Error;
        MessagePayload payload = ErrorPayload{};
    };

    std::string          toString(MessageType type);
    MessageType          parseMessageType(std::string_view type);
    std::string          toString(TradeCore::OrderType type);
    TradeCore::OrderType parseOrderType(std::string_view type);

    Envelope    makeSubmitOrderEnvelope(int clientId, TradeCore::OrderType side, int price, int volume);
    Envelope    makeTradeEnvelope(const TradeCore::Trade& trade);
    Envelope    makeErrorEnvelope(std::string message);
    std::string serializeEnvelope(const Envelope& envelope);
    Envelope    parseEnvelope(const std::string& payload);

    TradeCore::Order toOrder(const SubmitOrderPayload& payload);
    std::string      formatTimepoint(timepoint_t timepoint);
    std::string      formatTradeLine(const TradePayload& payload);
    std::string      formatTradeLine(const TradeCore::Trade& trade);
}
