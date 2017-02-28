#pragma once

#include "ImageReader.h"
#include "../lodepng/lodepng.h"
#include "RGBAPixel.h"

#include <cmath>
#include <iostream>


namespace {

template <class PixelType>
#if !_MSC_VER
__attribute__((unused))
#endif
void CastRGBPixels(const Image<RGBAPixel> &rgb, Image<PixelType> &img) {
  img.Resize(rgb.getHeight(), rgb.getWidth());

  for (size_t i = 0; i < rgb.getSize(); ++i) {
    img[i] =
      std::lround(0.21 * rgb[i].r + 0.72 * rgb[i].g + 0.07 * rgb[i].b);
  }
}

template<>
void CastRGBPixels<RGBAPixel>(const Image<RGBAPixel> &rgb,
                              Image<RGBAPixel> &img) {
  img = rgb;
}

} // namespace


namespace ImageIO {

template <class T>
class PngImageReader : public ImageReader<T> {
public:
  using Self       = PngImageReader<T>;
  using SuperClass = ImageReader<T>;

  PngImageReader(const std::string &fileName)
    : SuperClass(fileName)
  {}

  void Read() {
    std::vector<RGBAPixel::PixelType> png;
    std::vector<RGBAPixel::PixelType> image;
    unsigned width = 0, height = 0;

    if (lodepng::load_file(png, SuperClass::fileName))
      throw std::runtime_error("Error loading PNG file " +
                               SuperClass::fileName);

    if (lodepng::decode(image, width, height, png))
      throw std::runtime_error("Error decoding PNG file " +
                               SuperClass::fileName);

    Image<RGBAPixel> rgbImage(height, width);

    for (size_t i = 0; i < rgbImage.getSize(); ++i) {
      auto r = image[4 * i + 0];
      auto g = image[4 * i + 1];
      auto b = image[4 * i + 2];
      auto a = image[4 * i + 3];
      rgbImage[i] = RGBAPixel(r, g, b, a);
    }

    CastRGBPixels<T>(rgbImage, SuperClass::image);
  }
};

} // namespace ImageIO
