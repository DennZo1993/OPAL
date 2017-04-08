#pragma once

#include <string>
#include <utility>


namespace util {

// Splits string by the last occurence of the delimiter.
std::pair<std::string, std::string>
SplitStringByLast(const std::string &str, char delim);

} // namespace util

