#pragma once

#include <utility>
#include <string>
#include <fstream>


// Splits string by the last occurence of the delimiter.
std::pair<std::string, std::string>
SplitStringByLast(const std::string &str, char delim);


// Reads the contents of the file into std::string.
std::string ReadFileToString(const std::string &fileName);

std::string ReadFileToString(std::ifstream &ifs);
