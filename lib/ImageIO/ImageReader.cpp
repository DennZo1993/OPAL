#include "ImageReader.h"

namespace ImageIO {

std::pair<std::string, std::string>
SplitFileName(const std::string &fileName) {
  auto dotIndex = fileName.rfind(".");
  if (dotIndex == std::string::npos)
    return std::make_pair(fileName, "");
  auto name = fileName.substr(0, dotIndex);
  auto ext = fileName.substr(dotIndex + 1, fileName.length() - dotIndex);
  return std::make_pair(name, ext);
}


} // namespace ImageIO
