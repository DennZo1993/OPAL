#include "File.h"

#include <sstream>


namespace util {

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

} // namespace util

