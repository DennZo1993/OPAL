#include "PngImageWriter.h"

namespace ImageIO {

template<>
void PngImageWriter<RGBAPixel>::ConvertToRGB(const Image<RGBAPixel> &image) {
  for (size_t i = 0; i < image.getSize(); ++i) {
    rawPixels[4 * i + 0] = image[i].r;
    rawPixels[4 * i + 1] = image[i].g;
    rawPixels[4 * i + 2] = image[i].b;
    rawPixels[4 * i + 3] = image[i].a;
  }
}


} // namespace ImageIO
