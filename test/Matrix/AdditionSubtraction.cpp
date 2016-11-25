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

TEST(ArithmeticTest, AdditionDifferentSize1) {
  Image<double> m1;
  Image<double> m2(1, 1, 0.0);

  ASSERT_THROW(m2 + m1, std::invalid_argument);
  ASSERT_THROW(m1 += m2, std::invalid_argument);
  ASSERT_TRUE(ImageIsEmpty(m1));
}

TEST(ArithmeticTest, AdditionDifferentSize2) {
  Image<double> m1(1, 2, 1.3);
  Image<double> m2(2, 1, 0.0);

  ASSERT_THROW(m2 + m1, std::invalid_argument);
  ASSERT_THROW(m1 += m2, std::invalid_argument);

  // Check that matrix didn't change.
  ASSERT_TRUE(ImageHasSize(m1, 1, 2));
  ASSERT_TRUE(ImageIsFilledWith(m1, 1.3));
}

TEST(ArithmeticTest, AdditionDifferentSize3) {
  Image<int> m1(10, 10, 1);
  Image<int> m2(10, 9, 3);

  ASSERT_THROW(m2 + m1, std::invalid_argument);
  ASSERT_THROW(m1 += m2, std::invalid_argument);

  ASSERT_TRUE(ImageHasSize(m1, 10, 10));
  ASSERT_TRUE(ImageIsFilledWith(m1, 1));
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

TEST(ArithmeticTest, SubtractionDifferentSize1) {
  Image<double> m1;
  Image<double> m2(1, 1, 0.0);

  ASSERT_THROW(m2 - m1, std::invalid_argument);
  ASSERT_THROW(m1 -= m2, std::invalid_argument);
  ASSERT_TRUE(ImageIsEmpty(m1));
}

TEST(ArithmeticTest, SubtractionDifferentSize2) {
  Image<double> m1(1, 2, 1.3);
  Image<double> m2(2, 1, 0.0);

  ASSERT_THROW(m2 - m1, std::invalid_argument);
  ASSERT_THROW(m1 -= m2, std::invalid_argument);

  // Check that matrix didn't change.
  ASSERT_TRUE(ImageHasSize(m1, 1, 2));
  ASSERT_TRUE(ImageIsFilledWith(m1, 1.3));
}

TEST(ArithmeticTest, SubtractionDifferentSize3) {
  Image<int> m1(10, 10, 1);
  Image<int> m2(10, 9, 3);

  ASSERT_THROW(m2 - m1, std::invalid_argument);
  ASSERT_THROW(m1 -= m2, std::invalid_argument);

  ASSERT_TRUE(ImageHasSize(m1, 10, 10));
  ASSERT_TRUE(ImageIsFilledWith(m1, 1));
}

