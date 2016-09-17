#include "Common.h"

TEST(ArithmeticTest, AdditionEmpty1) {
  Matrix<int> m1;
  Matrix<int> m2;
  Matrix<int> m3 = m1 + m2;

  ASSERT_TRUE(MatrixIsEmpty(m3));
}

TEST(ArithmeticTest, AdditionEmpty2) {
  Matrix<int> m1;
  Matrix<int> m2;
  m1 += m2;

  ASSERT_TRUE(MatrixIsEmpty(m1));
}

TEST(ArithmeticTest, AdditionNonEmpty1) {
  Matrix<float> m1(5, 5, 1.0);
  Matrix<float> m2(5, 5, 2.0);
  Matrix<float> m3 = m1 + m2;

  ASSERT_TRUE(MatrixHasSize(m3, 5, 5));
  ASSERT_TRUE(MatrixIsFilledWith(m3, 3.0f));
}

TEST(ArithmeticTest, AdditionNonEmpty2) {
  Matrix<float> m1(5, 5, 1.0);
  Matrix<float> m2(5, 5, 2.0);
  m1 += m2;

  ASSERT_TRUE(MatrixHasSize(m1, 5, 5));
  ASSERT_TRUE(MatrixIsFilledWith(m1, 3.0f));
}

TEST(ArithmeticTest, AdditionDifferentSize1) {
  Matrix<double> m1;
  Matrix<double> m2(1, 1, 0.0);

  ASSERT_THROW(m2 + m1, std::invalid_argument);
  ASSERT_THROW(m1 += m2, std::invalid_argument);
  ASSERT_TRUE(MatrixIsEmpty(m1));
}

TEST(ArithmeticTest, AdditionDifferentSize2) {
  Matrix<double> m1(1, 2, 1.3);
  Matrix<double> m2(2, 1, 0.0);

  ASSERT_THROW(m2 + m1, std::invalid_argument);
  ASSERT_THROW(m1 += m2, std::invalid_argument);

  // Check that matrix didn't change.
  ASSERT_TRUE(MatrixHasSize(m1, 1, 2));
  ASSERT_TRUE(MatrixIsFilledWith(m1, 1.3));
}

TEST(ArithmeticTest, AdditionDifferentSize3) {
  Matrix<int> m1(10, 10, 1);
  Matrix<int> m2(10, 9, 3);

  ASSERT_THROW(m2 + m1, std::invalid_argument);
  ASSERT_THROW(m1 += m2, std::invalid_argument);

  ASSERT_TRUE(MatrixHasSize(m1, 10, 10));
  ASSERT_TRUE(MatrixIsFilledWith(m1, 1));
}


// --------------- Subtraction ----------------


TEST(ArithmeticTest, SubtractionEmpty1) {
  Matrix<int> m1;
  Matrix<int> m2;
  Matrix<int> m3 = m1 - m2;

  ASSERT_TRUE(MatrixIsEmpty(m3));
}

TEST(ArithmeticTest, SubtractionEmpty2) {
  Matrix<int> m1;
  Matrix<int> m2;
  m1 -= m2;

  ASSERT_TRUE(MatrixIsEmpty(m1));
}

TEST(ArithmeticTest, SubtractionNonEmpty1) {
  Matrix<float> m1(5, 5, 1.0);
  Matrix<float> m2(5, 5, 2.0);
  Matrix<float> m3 = m1 - m2;

  ASSERT_TRUE(MatrixHasSize(m3, 5, 5));
  ASSERT_TRUE(MatrixIsFilledWith(m3, -1.0f));
}

TEST(ArithmeticTest, SubtractionNonEmpty2) {
  Matrix<float> m1(5, 5, 1.0);
  Matrix<float> m2(5, 5, 2.0);
  m1 -= m2;

  ASSERT_TRUE(MatrixHasSize(m1, 5, 5));
  ASSERT_TRUE(MatrixIsFilledWith(m1, -1.0f));
}

TEST(ArithmeticTest, SubtractionDifferentSize1) {
  Matrix<double> m1;
  Matrix<double> m2(1, 1, 0.0);

  ASSERT_THROW(m2 - m1, std::invalid_argument);
  ASSERT_THROW(m1 -= m2, std::invalid_argument);
  ASSERT_TRUE(MatrixIsEmpty(m1));
}

TEST(ArithmeticTest, SubtractionDifferentSize2) {
  Matrix<double> m1(1, 2, 1.3);
  Matrix<double> m2(2, 1, 0.0);

  ASSERT_THROW(m2 - m1, std::invalid_argument);
  ASSERT_THROW(m1 -= m2, std::invalid_argument);

  // Check that matrix didn't change.
  ASSERT_TRUE(MatrixHasSize(m1, 1, 2));
  ASSERT_TRUE(MatrixIsFilledWith(m1, 1.3));
}

TEST(ArithmeticTest, SubtractionDifferentSize3) {
  Matrix<int> m1(10, 10, 1);
  Matrix<int> m2(10, 9, 3);

  ASSERT_THROW(m2 - m1, std::invalid_argument);
  ASSERT_THROW(m1 -= m2, std::invalid_argument);

  ASSERT_TRUE(MatrixHasSize(m1, 10, 10));
  ASSERT_TRUE(MatrixIsFilledWith(m1, 1));
}

