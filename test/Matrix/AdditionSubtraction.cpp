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

