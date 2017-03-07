#include "util.h"
#include <sstream>


std::pair<std::string, std::string>
SplitStringByLast(const std::string &str, char delim) {
  auto delimIndex = str.rfind(delim);
  if (delimIndex == std::string::npos)
    return std::make_pair(str, "");
  auto first = str.substr(0, delimIndex);
  auto second = str.substr(delimIndex + 1, str.length() - delimIndex);
  return std::make_pair(first, second);
}


// File-related functions.
std::string ReadFileToString(const std::string &fileName) {
  std::ifstream ifs(fileName);
  return ReadFileToString(ifs);
}


std::string ReadFileToString(std::ifstream &ifs) {
  std::stringstream buffer;
  buffer << ifs.rdbuf();
  return buffer.str();
}
