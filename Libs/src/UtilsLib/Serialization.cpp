#include "UtilsLib/Serialization.h"

TRANVANH_NAMESPACE_BEGIN

std::ostream& operator<<(std::ostream& os, const Serializable& object){
    return object.serialize(os);
}

TRANVANH_NAMESPACE_END
