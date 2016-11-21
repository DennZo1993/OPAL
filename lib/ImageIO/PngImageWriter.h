#pragma once

#include "ImageWriter.h"
#include "RGBAPixel.h"
#include "../lodepng/lodepng.h"
#include <vector>
#include <stdexcept>


namespace ImageIO {

// Generic PNG image writer.
template <class T>
class PngImageWriter : public ImageWriter<T> {
public:
  using Self       = PngImageWriter<T>;
  using SuperClass = ImageWriter<T>;

  PngImageWriter() = default;
  ~PngImageWriter() = default;

  virtual void Write(const Image<T> &image, const std::string &fileName) {
    rawPixels.resize(4 * image.getSize(), 0);
    ConvertToRGB(image);

    auto error =
      lodepng::encode(fileName, rawPixels, image.getWidth(), image.getHeight());
    if (error)
      throw std::runtime_error("Error while encoding file " + fileName);
  }

protected:
  virtual void ConvertToRGB(const Image<T> &image) {
    for (size_t i = 0; i < image.getSize(); ++i) {
      rawPixels[4 * i + 0] = rawPixels[4 * i + 1] = rawPixels[4 * i + 2] =
        image[i];
      rawPixels[4 * i + 3] = 255;
    }
  }

  std::vector<RGBAPixel::PixelType> rawPixels;
};


template<>
void PngImageWriter<RGBAPixel>::ConvertToRGB(const Image<RGBAPixel> &image);

} // namespace ImageIO
