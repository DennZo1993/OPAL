#pragma once

#include <string>
#include <fstream>


namespace util {

// Reads the contents of the file into std::string.
std::string ReadFileToString(const std::string &fileName);

std::string ReadFileToString(std::ifstream &ifs);

} // namespace util

