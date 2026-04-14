#include "TradeCommon/Validation.h"
#include <charconv>

namespace TradeCommon {

    std::optional<int> tryParseInt(const std::string_view value) {
        if (value.empty()) {
            return std::nullopt;
        }

        int        parsed = 0;
        const auto result = std::from_chars(value.data(), value.data() + value.size(), parsed);
        if (result.ec != std::errc{} || result.ptr != value.data() + value.size()) {
            return std::nullopt;
        }
        return parsed;
    }

    std::optional<unsigned short> tryParsePort(const std::string_view value) {
        const auto parsed = tryParseInt(value);
        if (!parsed || *parsed <= 0 || *parsed > 65535) {
            return std::nullopt;
        }
        return static_cast<unsigned short>(*parsed);
    }

    std::optional<std::string> validateClientId(const int clientId) {
        if (clientId <= 0) {
            return std::string("Client id must be a positive integer.");
        }
        return std::nullopt;
    }

    std::optional<std::string> validateOrder(const int price, const int volume) {
        if (price <= 0) {
            return std::string("Price must be a positive integer.");
        }
        if (volume <= 0) {
            return std::string("Volume must be a positive integer.");
        }
        return std::nullopt;
    }
}
