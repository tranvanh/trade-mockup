#pragma once
#include <iostream>
#include <chrono>
#include <iomanip>

using timepoint_t = std::chrono::time_point<std::chrono::system_clock>;
std::ostream& operator<<(std::ostream& os, const timepoint_t& timepoint);