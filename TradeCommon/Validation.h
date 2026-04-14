#pragma once
#include <optional>
#include <string>
#include <string_view>

namespace TradeCommon {

    std::optional<int>            tryParseInt(std::string_view value);
    std::optional<unsigned short> tryParsePort(std::string_view value);
    std::optional<std::string>    validateClientId(int clientId);
    std::optional<std::string>    validateOrder(int price, int volume);
}
