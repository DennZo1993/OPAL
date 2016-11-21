#pragma once

#include "Image.h"
#include <string>


namespace ImageIO {

template <class T>
class ImageWriter {
public:
  ImageWriter() = default;
  virtual ~ImageWriter() = default;

  virtual void Write(const Image<T> &image, const std::string &fileName) = 0;
};

} // namespace ImageIO
