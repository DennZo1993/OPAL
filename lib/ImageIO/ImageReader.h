#pragma once

#include "Image.h"
#include <string>


namespace ImageIO {

template <class T>
class ImageReader {
public:
  ImageReader(const std::string &file)
    : fileName(file)
    , image()
  {}

  virtual ~ImageReader() = default;

  virtual void Read() = 0;

  void SetFileName(const std::string &file) { fileName = file; }
  const Image<T> & GetImage() const { return image; }

protected:
  std::string fileName;
  Image<T> image;
};


std::pair<std::string, std::string>
SplitFileName(const std::string &fileName);

} // namespace ImageIO
