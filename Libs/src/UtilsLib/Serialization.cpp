#include "UtilsLib/Serialization.h"

TRANVANH_NAMESPACE_BEGIN

std::ostream& operator<<(std::ostream& os, const timepoint_t& timepoint) {
    auto tp = std::chrono::system_clock::to_time_t(timepoint);
    os << std::put_time(std::localtime(&tp), "%F %T");
    // Compute milliseconds
    auto ms = duration_cast<std::chrono::milliseconds>(timepoint.time_since_epoch()) % 1000;
    // Append milliseconds
    os << '.' << std::setw(3) << std::setfill('0') << ms.count();
    return os;
}

std::ostream& operator<<(std::ostream& os, const Serializable& object){
    return object.serialize(os);
}

TRANVANH_NAMESPACE_END
