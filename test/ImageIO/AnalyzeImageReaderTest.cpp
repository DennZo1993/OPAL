#include <gtest/gtest.h>

#include "ImageIO/AnalyzeImageReader.h"

using namespace ImageIO;

TEST(AnalyzeImageReader, ReadGood) {
  AnalyzeImageReader<int> reader("test_data/Images/01.img");
  reader.Read();

  const auto &image = reader.GetImage();
  ASSERT_EQ(256, image.getHeight());
  ASSERT_EQ(256, image.getWidth());
}


TEST(AnalyzeImageReader, ReadBad) {
  AnalyzeImageReader<int> reader("some_weird_name");
  ASSERT_THROW(reader.Read(), std::runtime_error);
}


TEST(AnalyzeImageReader, ReadBadNoHeader) {
  AnalyzeImageReader<int> reader("test_data/01.img");
  ASSERT_THROW(reader.Read(), std::runtime_error);
}

