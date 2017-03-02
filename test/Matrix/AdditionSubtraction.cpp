#include "../Common.h"

TEST(ArithmeticTest, AdditionEmpty1) {
  Image<int> m1;
  Image<int> m2;
  Image<int> m3 = m1 + m2;

  ASSERT_TRUE(ImageIsEmpty(m3));
}

TEST(ArithmeticTest, AdditionEmpty2) {
  Image<int> m1;
  Image<int> m2;
  m1 += m2;

  ASSERT_TRUE(ImageIsEmpty(m1));
}

TEST(ArithmeticTest, AdditionNonEmpty1) {
  Image<float> m1(5, 5, 1.0);
  Image<float> m2(5, 5, 2.0);
  Image<float> m3 = m1 + m2;

  ASSERT_TRUE(ImageHasSize(m3, 5, 5));
  ASSERT_TRUE(ImageIsFilledWith(m3, 3.0f));
}

TEST(ArithmeticTest, AdditionNonEmpty2) {
  Image<float> m1(5, 5, 1.0);
  Image<float> m2(5, 5, 2.0);
  m1 += m2;

  ASSERT_TRUE(ImageHasSize(m1, 5, 5));
  ASSERT_TRUE(ImageIsFilledWith(m1, 3.0f));
}


// --------------- Subtraction ----------------


TEST(ArithmeticTest, SubtractionEmpty1) {
  Image<int> m1;
  Image<int> m2;
  Image<int> m3 = m1 - m2;

  ASSERT_TRUE(ImageIsEmpty(m3));
}

TEST(ArithmeticTest, SubtractionEmpty2) {
  Image<int> m1;
  Image<int> m2;
  m1 -= m2;

  ASSERT_TRUE(ImageIsEmpty(m1));
}

TEST(ArithmeticTest, SubtractionNonEmpty1) {
  Image<float> m1(5, 5, 1.0);
  Image<float> m2(5, 5, 2.0);
  Image<float> m3 = m1 - m2;

  ASSERT_TRUE(ImageHasSize(m3, 5, 5));
  ASSERT_TRUE(ImageIsFilledWith(m3, -1.0f));
}

TEST(ArithmeticTest, SubtractionNonEmpty2) {
  Image<float> m1(5, 5, 1.0);
  Image<float> m2(5, 5, 2.0);
  m1 -= m2;

  ASSERT_TRUE(ImageHasSize(m1, 5, 5));
  ASSERT_TRUE(ImageIsFilledWith(m1, -1.0f));
}

