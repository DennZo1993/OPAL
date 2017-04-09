#include "gtest/gtest.h"
#include "Image.h"

#include <sstream>


TEST(ImageTest, StreamInputOutput) {
  // 5x6 filled with 18.
  Image<int> written(5, 6, 18);

  std::stringstream ss;
  ss << written;

  Image<int> read;
  ss >> read;

  ASSERT_EQ(written, read);
}


TEST(ImageTest, StreamInputOutputDouble) {
  Image<double> written(7, 2, 4.579);

  std::stringstream ss;
  ss << written;

  Image<double> read;
  ss >> read;

  ASSERT_EQ(written, read);
}
