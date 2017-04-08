#include "Directory.h"

#include <algorithm>


namespace util {

// Directory-related functions
std::vector<std::string> ListDir(const std::string &dirName,
                                 const std::string &extension,
                                 bool sorted)
{
  namespace fs = std::experimental::filesystem;

  std::vector<std::string> result;
  for (const auto &f : fs::directory_iterator(dirName)) {
    auto p = f.path();
    // If extension is given, see if it matches.
    if (extension != "" && p.extension() != extension)
      continue;
    result.push_back(p.string());
  }

  if (sorted)
    std::sort(result.begin(), result.end());

  return result;
}

} // namespace util

