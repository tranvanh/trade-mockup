#include "TradeCommon/Protocol.h"
#include <nlohmann/json.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace TradeCommon {
    namespace {
        using json = nlohmann::json;

        std::string formatTradeLine(const int buyerId,
                                    const int sellerId,
                                    const int price,
                                    const int volume,
                                    const std::string_view tradeTime) {
            std::ostringstream oss;
            oss << tradeTime << " buyer=" << buyerId << " seller=" << sellerId << " price=" << price
                << " volume=" << volume;
            return oss.str();
        }
    }

    std::string toString(const MessageType type) {
        switch (type) {
        case MessageType::SubmitOrder:
            return "submit_order";
        case MessageType::Trade:
            return "trade";
        case MessageType::Error:
            return "error";
        }
        throw std::invalid_argument("Unknown message type");
    }

    MessageType parseMessageType(const std::string_view type) {
        if (type == "submit_order") {
            return MessageType::SubmitOrder;
        }
        if (type == "trade") {
            return MessageType::Trade;
        }
        if (type == "error") {
            return MessageType::Error;
        }
        throw std::invalid_argument("Unsupported message type");
    }

    std::string toString(const TradeCore::OrderType type) {
        switch (type) {
        case TradeCore::OrderType::BUY:
            return "buy";
        case TradeCore::OrderType::SELL:
            return "sell";
        }
        throw std::invalid_argument("Unsupported order type");
    }

    TradeCore::OrderType parseOrderType(const std::string_view type) {
        if (type == "buy") {
            return TradeCore::OrderType::BUY;
        }
        if (type == "sell") {
            return TradeCore::OrderType::SELL;
        }
        throw std::invalid_argument("Unsupported order side");
    }

    Envelope makeSubmitOrderEnvelope(const int clientId,
                                     const TradeCore::OrderType side,
                                     const int price,
                                     const int volume) {
        return Envelope{MessageType::SubmitOrder, SubmitOrderPayload{clientId, side, price, volume}};
    }

    Envelope makeTradeEnvelope(const TradeCore::Trade& trade) {
        return Envelope{MessageType::Trade,
                        TradePayload{trade.sellerId,
                                     trade.buyerId,
                                     trade.price,
                                     trade.volume,
                                     formatTimepoint(trade.tradeTime)}};
    }

    Envelope makeErrorEnvelope(std::string message) {
        return Envelope{MessageType::Error, ErrorPayload{std::move(message)}};
    }

    std::string serializeEnvelope(const Envelope& envelope) {
        json root;
        root["type"] = toString(envelope.type);

        json payload;
        switch (envelope.type) {
        case MessageType::SubmitOrder: {
            const auto& submit = std::get<SubmitOrderPayload>(envelope.payload);
            payload["clientId"] = submit.clientId;
            payload["side"]     = toString(submit.side);
            payload["price"]    = submit.price;
            payload["volume"]   = submit.volume;
            break;
        }
        case MessageType::Trade: {
            const auto& trade = std::get<TradePayload>(envelope.payload);
            payload["sellerId"]  = trade.sellerId;
            payload["buyerId"]   = trade.buyerId;
            payload["price"]     = trade.price;
            payload["volume"]    = trade.volume;
            payload["tradeTime"] = trade.tradeTime;
            break;
        }
        case MessageType::Error: {
            const auto& error = std::get<ErrorPayload>(envelope.payload);
            payload["message"] = error.message;
            break;
        }
        }

        root["payload"] = std::move(payload);
        return root.dump();
    }

    Envelope parseEnvelope(const std::string& payload) {
        const auto root    = json::parse(payload);
        const auto type    = parseMessageType(root.at("type").get<std::string>());
        const auto& body   = root.at("payload");

        switch (type) {
        case MessageType::SubmitOrder:
            return Envelope{type,
                            SubmitOrderPayload{body.at("clientId").get<int>(),
                                               parseOrderType(body.at("side").get<std::string>()),
                                               body.at("price").get<int>(),
                                               body.at("volume").get<int>()}};
        case MessageType::Trade:
            return Envelope{type,
                            TradePayload{body.at("sellerId").get<int>(),
                                         body.at("buyerId").get<int>(),
                                         body.at("price").get<int>(),
                                         body.at("volume").get<int>(),
                                         body.at("tradeTime").get<std::string>()}};
        case MessageType::Error:
            return Envelope{type, ErrorPayload{body.at("message").get<std::string>()}};
        }

        throw std::invalid_argument("Unhandled envelope type");
    }

    TradeCore::Order toOrder(const SubmitOrderPayload& payload) {
        return TradeCore::Order(payload.clientId, payload.side, payload.price, payload.volume);
    }

    std::string formatTimepoint(const timepoint_t timepoint) {
        const auto time = std::chrono::system_clock::to_time_t(timepoint);
        std::tm     tm {};
        localtime_r(&time, &tm);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%F %T");
        const auto milliseconds =
            std::chrono::duration_cast<std::chrono::milliseconds>(timepoint.time_since_epoch()) % 1000;
        oss << '.' << std::setw(3) << std::setfill('0') << milliseconds.count();
        return oss.str();
    }

    std::string formatTradeLine(const TradePayload& payload) {
        return formatTradeLine(payload.buyerId,
                               payload.sellerId,
                               payload.price,
                               payload.volume,
                               payload.tradeTime);
    }

    std::string formatTradeLine(const TradeCore::Trade& trade) {
        return formatTradeLine(trade.buyerId,
                               trade.sellerId,
                               trade.price,
                               trade.volume,
                               formatTimepoint(trade.tradeTime));
    }
}
