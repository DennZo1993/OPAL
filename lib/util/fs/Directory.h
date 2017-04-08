#pragma once

#include <experimental/filesystem>
#include <string>
#include <vector>


namespace util {

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

} // namespace util

