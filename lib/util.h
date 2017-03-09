#pragma once

#include <utility>
#include <string>
#include <fstream>
#include <vector>
#include <experimental/filesystem>


namespace util
{

// Splits string by the last occurence of the delimiter.
std::pair<std::string, std::string>
SplitStringByLast(const std::string &str, char delim);


// List contents orf the directory.
std::vector<std::string> ListDir(const std::string &dirName,
                                 const std::string &extension = "",
                                 bool sorted = false);


inline std::string ConcatPaths(const std::string &path)
{
  return path;
}


template<class... Types>
inline std::string ConcatPaths(const std::string &first, Types... args)
{
  namespace fs = std::experimental::filesystem;

  auto firstPath = fs::path(first);
  firstPath /= ConcatPaths(args...);
  return firstPath.string();
}


// Reads the contents of the file into std::string.
std::string ReadFileToString(const std::string &fileName);

std::string ReadFileToString(std::ifstream &ifs);

} // namespace util
