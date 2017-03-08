#include "util.h"

#include <sstream>
#include <algorithm>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;


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


// Directory-related functions
std::vector<std::string> ListDir(const std::string &dirName,
                                 bool sorted)
{
  std::vector<std::string> result;
  for (const auto &p : fs::directory_iterator(dirName))
    result.push_back(p.path().string());

  if (sorted)
    std::sort(result.begin(), result.end());

  return result;
}


// File-related functions.
std::string ReadFileToString(const std::string &fileName)
{
  std::ifstream ifs(fileName);
  return ReadFileToString(ifs);
}


std::string ReadFileToString(std::ifstream &ifs)
{
  std::stringstream buffer;
  buffer << ifs.rdbuf();
  return buffer.str();
}
