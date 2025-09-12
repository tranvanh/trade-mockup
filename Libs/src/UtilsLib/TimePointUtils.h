#pragma once
#include "UtilsLib/Common.h"
#include <iostream>
#include <chrono>
#include <iomanip>

TRANVANH_NAMESPACE_BEGIN

using timepoint_t = std::chrono::time_point<std::chrono::system_clock>;
std::ostream& operator<<(std::ostream& os, const timepoint_t& timepoint);

TRANVANH_NAMESPACE_END
