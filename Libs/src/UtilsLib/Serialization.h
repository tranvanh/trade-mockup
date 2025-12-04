#pragma once
#include "Common.h"
#include <sstream>

TRANVANH_NAMESPACE_BEGIN

class Serializable {
protected:
    virtual ~Serializable() = default;
    virtual std::ostream& serialize(std::ostream& os) const = 0;

public:
    friend std::ostream& operator<<(std::ostream& os, const Serializable& trade);
};

TRANVANH_NAMESPACE_END
