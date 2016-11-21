#include "ImageReader.h"
//#include <vector>
//#include <stdexcept>


namespace ImageIO {

template <class T>
Image<T> ReadImage(const std::string &fileName) {
  auto fileNameParts = SplitFileName(fileName);

  ImageReader<T> *reader = nullptr;

  if (fileNameParts.second == "img")
    reader = new AnalyzeImageReader(fileName);
  else if (fileNameParts.second == "png")
    reader = new PngImageReader(fileName);

  if (!reader)
    return Image<T>();

  reader->Read();
  return reader->GetImage();
}

#if 0
// Abstract PNG file writer.
template <class ImageType>
class PNGImageWriter {
public:
  PNGImageWriter(const ImageType &img, const std::string &file)
    : image(img)
    , fileName(file)
    , rawPixels(4 * img.getHeight() * img.getWidth(), 0)
  {
  }

  void Write() {
    ConvertToPNG();

    auto error =
      lodepng::encode(fileName, rawPixels, image.getWidth(), image.getHeight());
    if (error)
      throw std::runtime_error("Error while encoding file " + fileName);
  }

protected:
  virtual void ConvertToPNG() = 0;

  const ImageType &image;
  std::string fileName;
  std::vector<RGBAPixel::PixelType> rawPixels;
};


// Writer for RGB images.
class RGBImageWriter : public PNGImageWriter<RGBImage> {
public:
  RGBImageWriter(const RGBImage &img, const std::string &file)
    : PNGImageWriter<RGBImage>(img, file) {}

protected:
  void ConvertToPNG() override final {
    for (size_t i = 0; i < image.getSize(); ++i) {
      rawPixels[4 * i + 0] = image[i].r;
      rawPixels[4 * i + 1] = image[i].g;
      rawPixels[4 * i + 2] = image[i].b;
      rawPixels[4 * i + 3] = image[i].a;
    }
  }
};


// Writer for binary images.
class BinaryImageWriter : public PNGImageWriter<BinaryImage> {
public:
  BinaryImageWriter(const BinaryImage &img, const std::string &file)
    : PNGImageWriter<BinaryImage>(img, file) {}

protected:
  void ConvertToPNG() override final {
    for (size_t i = 0; i < image.getSize(); ++i) {
      RGBAPixel::PixelType value = 255 * image[i];
      rawPixels[4 * i + 0] = rawPixels[4 * i + 1] = rawPixels[4 * i + 2] =
        value;
      rawPixels[4 * i + 3] = 255;
    }
  }
};


// Writer for gray scale images.
class GrayScaleImageWriter : public PNGImageWriter<GrayScaleImage> {
public:
  GrayScaleImageWriter(const GrayScaleImage &img, const std::string &file)
    : PNGImageWriter<GrayScaleImage>(img, file) {}

protected:
  void ConvertToPNG() override final {
    for (size_t i = 0; i < image.getSize(); ++i) {
      rawPixels[4 * i + 0] = rawPixels[4 * i + 1] = rawPixels[4 * i + 2] =
        image[i];
      rawPixels[4 * i + 3] = 255;
    }
  }
};


// Write RGB image.
void WriteImage(const RGBImage &image, const std::string &fileName) {
  RGBImageWriter writer(image, fileName);
  writer.Write();
}

// Write binary image.
void WriteImage(const BinaryImage &image, const std::string &fileName) {
  BinaryImageWriter writer(image, fileName);
  writer.Write();
}

// Write gray scale image.
void WriteImage(const GrayScaleImage &image, const std::string &fileName) {
  GrayScaleImageWriter writer(image, fileName);
  writer.Write();
}
#endif
} // namespace ImageIO
