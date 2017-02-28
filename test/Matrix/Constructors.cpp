#include "../Common.h"

Image<double> GetImage() {
  return Image<double>(5, 10, 7.0);
}

TEST(ImageTest, Constructors1) {
  Image<double> m1;
  ASSERT_TRUE(ImageIsEmpty(m1));
}

TEST(ImageTest, Constructors2) {
  Image<int> m2(0, 20, 3);
  ASSERT_TRUE(ImageIsEmpty(m2));
}

TEST(ImageTest, Constructors3) {
  Image<int> m3(10, 0, 0);
  ASSERT_TRUE(ImageIsEmpty(m3));
}

TEST(ImageTest, Constructors4) {
  Image<int> m4(0, 0, 0);
  ASSERT_TRUE(ImageIsEmpty(m4));
}

TEST(ImageTest, Constructors5) {
  // Copy-construction from empty matrix
  Image<int> m2;
  Image<int> m5(m2);
  ASSERT_TRUE(ImageIsEmpty(m5));
}

TEST(ImageTest, Constructors6) {
  // Non-empty matrix
  Image<int> m6(5, 10, 2);
  ASSERT_FALSE(m6.isEmpty());
  ASSERT_TRUE(ImageHasSize(m6, 5, 10));
  ASSERT_TRUE(ImageIsFilledWith(m6, 2));
}

TEST(ImageTest, Constructors7) {
  // Copy-construction from non-empty matrix
  Image<int> m6(5, 10, 2);
  Image<int> m7(m6);
  ASSERT_FALSE(m7.isEmpty());
  ASSERT_TRUE(ImageHasSize(m7, 5, 10));
  ASSERT_TRUE(ImageIsFilledWith(m7, 2));
}

TEST(ImageTest, Constructors9) {
  // Test move constructor
  Image<double> m9(GetImage());
  ASSERT_FALSE(m9.isEmpty());
  ASSERT_TRUE(ImageHasSize(m9, 5, 10));
  ASSERT_TRUE(ImageIsFilledWith(m9, 7.0));
}
