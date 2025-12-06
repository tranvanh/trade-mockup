#pragma once

#include "UtilsLib/Common.h"
#include "UtilsLib/Serialization.h"

TRANVANH_NAMESPACE_BEGIN

constexpr int PRICE_MAX  = 10000;
constexpr int VOLUME_MAX = 1000;

enum class OrderType { BUY = 0, SELL = 1 };

std::ostream& operator<<(std::ostream& os, OrderType type);

struct Order : public Serializable {
    int         clientId = 0;
    OrderType   type;
    int         price = 0;
    int         volume = 0;
    timepoint_t timeStamp;

    Order(const int id, const OrderType type, const int price, const int volume)
        : clientId(id)
        , type(type)
        , price(price)
        , volume(volume)
        , timeStamp(std::chrono::system_clock::now()) {}

    virtual std::ostream& serialize(std::ostream& os) const override;
};

TRANVANH_NAMESPACE_END
