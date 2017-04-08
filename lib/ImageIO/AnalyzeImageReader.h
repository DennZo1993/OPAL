#pragma once

#include "AnalyzeHeader.h"
#include "ImageReader.h"
#include "util/string/Split.h"

#include <cstdio>
#include <stdexcept>
#include <iostream>
#include <map>
#include <functional>


namespace ImageIO {

template <class T>
class AnalyzeImageReader : public ImageReader<T> {
public:
  using Self       = AnalyzeImageReader<T>;
  using SuperClass = ImageReader<T>;

  // Inherit constructor.
  using SuperClass::SuperClass;

  virtual void Read() override {
    ReadHdrFile();
    ReadImgFile();
  }

private:
  void ReadHdrFile() {
    auto hdrFileName = GetHeaderFileName();

    FILE *headerInput = fopen(hdrFileName.c_str(), "r");
    if (!headerInput)
      throw std::runtime_error("Cannot open hdr file for reading!");

    header.ReadFromFile(headerInput);
  }


  template <class DataType>
  static T ReadPixelValue(FILE *in, size_t size) {
    assert(in && "Input file is null!");
    DataType result;
    if (fread(&result, size, 1, in) != 1)
      throw std::runtime_error("Cannot read pixel from file!");

    return static_cast<T>(result);
  }


  void ReadImgFile() {
    FILE *imgInput = fopen(SuperClass::fileName.c_str(), "r");
    if (!imgInput)
      throw std::runtime_error("Cannot open img file for reading!");

    // Resolve reader for pixel type.
    using ReaderType = std::function<T(FILE*, size_t)>;
    std::map<int16_t, ReaderType> ReaderFuncMap;
    ReaderFuncMap[2]  = & ReadPixelValue<unsigned char>;
    ReaderFuncMap[4]  = & ReadPixelValue<int16_t>;
    ReaderFuncMap[8]  = & ReadPixelValue<int32_t>;
    ReaderFuncMap[16] = & ReadPixelValue<float>;
    ReaderFuncMap[64] = & ReadPixelValue<double>;

    auto headerDims = header.GetData().dims;
    auto PixelReader = ReaderFuncMap[headerDims.DataType];
    size_t width  = headerDims.Dimensions[1];
    size_t height = headerDims.Dimensions[2];
    SuperClass::image.Resize(height, width);

    for (size_t i = 0; i < width * height; ++i) {
      SuperClass::image[i] = PixelReader(imgInput, headerDims.BitsPerPixel / 8);
    }
  }


  std::string GetHeaderFileName() const {
    auto p = util::SplitStringByLast(SuperClass::fileName, '.');
    return p.first + ".hdr";
  }

private:
  Analyze::AnalyzeFileHeader header;
};

} // namespace ImageIO
