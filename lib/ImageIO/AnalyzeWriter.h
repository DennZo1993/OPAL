#pragma once

#include "ImageWriter.h"
#include "AnalyzeHeader.h"


namespace ImageIO {

template <class T>
class AnalyzeImageWriter : public ImageWriter<T> {
public:
  AnalyzeImageWriter(const AnalyzeFileHeader &hdr)
    : header(hdr)
  {}

  ~AnalyzeImageWriter() = default;

  void SetHeader(const AnalyzeFileHeader &hdr) {
    header = hdr;
  }

  virtual void Write(const Image<T> &image, const std::string &filename) {

  }

private:
  AnalyzeFileHeader header;
};

} // namespace ImageIO
