#pragma once

#include "UtilsLib/Common.h"
#include "UtilsLib/Serialization.h"

TRANVANH_NAMESPACE_BEGIN

constexpr int PRICE_MAX  = 10000;
constexpr int VOLUME_MAX = 1000;

enum class OrderType { BUY = 0, SELL = 1};

std::ostream& operator<<(std::ostream& os, OrderType type);

struct Order : public Serializable {
    int    clientId = 0;
    timepoint_t timeStamp;
    OrderType   type;
    int         price;
    int         volume;

    Order(const uint64_t id, const timepoint_t ts, const OrderType type, const int price, const int volume)
        : clientId(id)
        , timeStamp(ts)
        , type(type)
        , price(price)
        , volume(volume) {}

    virtual std::ostream& serialize(std::ostream& os) const override;
};

TRANVANH_NAMESPACE_END

