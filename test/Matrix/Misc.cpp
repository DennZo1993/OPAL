#include "../Common.h"

TEST(MatrixMiscTest, TestCast) {
  // Test casting non-empty matrix. Floor.
  Matrix<double> m1(2, 3, 1.3);
  Matrix<int> m2 = m1.castTo<int>();

  ASSERT_TRUE(MatrixHasSize(m2, 2, 3));
  ASSERT_TRUE(MatrixIsFilledWith(m2, 1));
}

TEST(MatrixMiscTest, TestCastEmpty)  {
  // Test casting empty matrix.
  Matrix<double> m1;
  Matrix<int> m2 = m1.castTo<int>();

  ASSERT_TRUE(MatrixIsEmpty(m2));
}

TEST(MatrixMiscTest, TestFillEmpty) {
  Matrix<float> m1;
  m1.Fill(5.0);

  ASSERT_TRUE(MatrixIsEmpty(m1));
}

TEST(MatrixMiscTest, TestFillNonEmpty) {
  Matrix<int> m1(3, 4, 2);
  m1.Fill(10);

  ASSERT_TRUE(MatrixHasSize(m1, 3, 4));
  ASSERT_TRUE(MatrixIsFilledWith(m1, 10));
}

TEST(MatrixMiscTest, TestAccessFail1) {
  Matrix<int> m1(5, 10, 3);

  ASSERT_THROW(m1[6][0], std::out_of_range);
  ASSERT_THROW(m1[5][9], std::out_of_range);
  ASSERT_THROW(m1[-1][0], std::out_of_range);
}

