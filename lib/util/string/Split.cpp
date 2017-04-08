#include "Split.h"


namespace util {

std::pair<std::string, std::string>
SplitStringByLast(const std::string &str, char delim)
{
  auto delimIndex = str.rfind(delim);
  if (delimIndex == std::string::npos)
    return std::make_pair(str, "");
  auto first = str.substr(0, delimIndex);
  auto second = str.substr(delimIndex + 1, str.length() - delimIndex);
  return std::make_pair(first, second);
}

} // namespace util

