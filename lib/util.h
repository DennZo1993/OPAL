#pragma once

#include <utility>
#include <string>
#include <fstream>
#include <vector>


// Splits string by the last occurence of the delimiter.
std::pair<std::string, std::string>
SplitStringByLast(const std::string &str, char delim);


// List contents orf the directory.
std::vector<std::string> ListDir(const std::string &dirName,
                                 bool sorted = false);


// Reads the contents of the file into std::string.
std::string ReadFileToString(const std::string &fileName);

std::string ReadFileToString(std::ifstream &ifs);
