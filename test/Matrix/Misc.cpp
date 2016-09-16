#include "Common.h"

TEST(MatrixTest, TestCast) {
  // Test casting non-empty matrix. Floor.
  Matrix<double> m1(2, 3, 1.3);
  Matrix<int> m2 = m1.castTo<int>();

  ASSERT_TRUE(MatrixHasSize(m2, 2, 3));
  ASSERT_TRUE(MatrixIsFilledWith(m2, 1));
}

TEST(MatrixTest, TestCastEmpty)  {
  // Test casting empty matrix.
  Matrix<double> m1;
  Matrix<int> m2 = m1.castTo<int>();

  ASSERT_TRUE(MatrixIsEmpty(m2));
}

TEST(MatrixTest, TestFillEmpty) {
  Matrix<float> m1;
  m1.Fill(5.0);

  ASSERT_TRUE(MatrixIsEmpty(m1));
}

TEST(MatrixTest, TestFillNonEmpty) {
  Matrix<int> m1(3, 4, 2);
  m1.Fill(10);

  ASSERT_TRUE(MatrixHasSize(m1, 3, 4));
  ASSERT_TRUE(MatrixIsFilledWith(m1, 10));
}
