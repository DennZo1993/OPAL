#pragma once

#include "AnalyzeImageReader.h"
#include "PngImageReader.h"

#include "PngImageWriter.h"


namespace ImageIO {

template <class T>
Image<T> ReadImage(const std::string &fileName) {
  auto fileNameParts = SplitFileName(fileName);

  ImageReader<T> *reader = nullptr;

  if (fileNameParts.second == "img")
    reader = new AnalyzeImageReader<T>(fileName);
  else if (fileNameParts.second == "png")
    reader = new PngImageReader<T>(fileName);

  if (!reader)
    return Image<T>();

  reader->Read();

  auto result = reader->GetImage();

  delete reader;
  return result;
}


template <class T>
void WriteImage(const Image<T> &image, const std::string &fileName) {
  auto fileNameParts = SplitFileName(fileName);

  ImageWriter<T> *writer = nullptr;

  if (fileNameParts.second == "png")
    writer = new PngImageWriter<T>;

  if (!writer)
    return;

  writer->Write(image, fileName);

  delete writer;
}

} // namespace ImageIO
