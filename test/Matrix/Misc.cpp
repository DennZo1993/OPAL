#include "../Common.h"

TEST(ImageMiscTest, TestCast) {
  // Test casting non-empty matrix. Floor.
  Image<double> m1(2, 3, 1.3);
  Image<int> m2 = m1.castTo<int>();

  ASSERT_TRUE(ImageHasSize(m2, 2, 3));
  ASSERT_TRUE(ImageIsFilledWith(m2, 1));
}

TEST(ImageMiscTest, TestCastEmpty)  {
  // Test casting empty matrix.
  Image<double> m1;
  Image<int> m2 = m1.castTo<int>();

  ASSERT_TRUE(ImageIsEmpty(m2));
}

TEST(ImageMiscTest, TestFillEmpty) {
  Image<float> m1;
  m1.Fill(5.0);

  ASSERT_TRUE(ImageIsEmpty(m1));
}

TEST(ImageMiscTest, TestFillNonEmpty) {
  Image<int> m1(3, 4, 2);
  m1.Fill(10);

  ASSERT_TRUE(ImageHasSize(m1, 3, 4));
  ASSERT_TRUE(ImageIsFilledWith(m1, 10));
}

TEST(ImageMiscTest, TestAccessFail1) {
  Image<int> m1(5, 10, 3);

  ASSERT_THROW(m1(6, 0), std::out_of_range);
  ASSERT_THROW(m1(5, 9), std::out_of_range);
  ASSERT_THROW(m1(-1, 0), std::out_of_range);
}

