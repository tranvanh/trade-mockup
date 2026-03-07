#include "Toybox/Serialization.h"

std::ostream& operator<<(std::ostream& os, const timepoint_t& timepoint) {
    auto tp = std::chrono::system_clock::to_time_t(timepoint);
    os << std::put_time(std::localtime(&tp), "%F %T");
    // Compute milliseconds
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(timepoint.time_since_epoch()) % 1000;
    // Append milliseconds
    os << '.' << std::setw(3) << std::setfill('0') << ms.count();
    return os;
}


std::ostream& operator<<(std::ostream& os, const toybox::Serializable& object){
    return object.serialize(os);
}

