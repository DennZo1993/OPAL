#include <gtest/gtest.h>

#include "ImageIO/PngImageReader.h"

using namespace ImageIO;

TEST(PngImageReader, ReadGood) {
  PngImageReader<int> reader("test_data/pictures/small_4x5_color.png");
  reader.Read();

  const auto &image = reader.GetImage();
  ASSERT_EQ(4, image.getHeight());
  ASSERT_EQ(5, image.getWidth());
}


TEST(PngImageReader, ReadBad) {
  PngImageReader<int> reader("some_weird_name");
  ASSERT_THROW(reader.Read(), std::runtime_error);
}


TEST(PngImageReader, ReRead) {
  PngImageReader<int> reader("test_data/pictures/small_4x5_color.png");
  reader.Read();

  ASSERT_EQ(4, reader.GetImage().getHeight());
  ASSERT_EQ(5, reader.GetImage().getWidth());

  reader.SetFileName("test_data/pictures/alley_1_frame_0001.png");
  reader.Read();

  ASSERT_EQ(436, reader.GetImage().getHeight());
  ASSERT_EQ(1024, reader.GetImage().getWidth());

  reader.SetFileName("test_data/pictures/small_4x5_color.png");
  reader.Read();

  ASSERT_EQ(4, reader.GetImage().getHeight());
  ASSERT_EQ(5, reader.GetImage().getWidth());

}
