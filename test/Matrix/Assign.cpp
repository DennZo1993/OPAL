#include "../Common.h"

TEST(AssignTest, AssignEmptyToEmpty) {
  Matrix<unsigned> m1;
  Matrix<unsigned> m2;
  m1 = m2;

  ASSERT_TRUE(MatrixIsEmpty(m1));
  ASSERT_TRUE(MatrixIsEmpty(m2));
}

TEST(AssignTest, AssignEmptyToNonEmpty1) {
  Matrix<int> m1(10, 10, 9);
  Matrix<int> m2;
  m1 = m2;

  ASSERT_TRUE(MatrixIsEmpty(m1));
  ASSERT_TRUE(MatrixIsEmpty(m2));
}

TEST(AssignTest, AssignEmptyToNonEmpty2) {
  Matrix<int> m1(10, 10, 9);
  Matrix<int> m2(2, 4, 7);
  Matrix<int> m3;
  m1 = m2 = m3;

  ASSERT_TRUE(MatrixIsEmpty(m1));
  ASSERT_TRUE(MatrixIsEmpty(m2));
  ASSERT_TRUE(MatrixIsEmpty(m3));
}

TEST(AssignTest, AssignNonEmptyToEmpty) {
  Matrix<double> m1;
  Matrix<double> m2(1, 1, 1.0);
  m1 = m2;

  ASSERT_FALSE(m1.isEmpty());
  ASSERT_TRUE(MatrixHasSize(m1, 1, 1));
  ASSERT_TRUE(MatrixIsFilledWith(m1, 1.0));
}

TEST(AssignTest, AssignNonEmpties) {
  Matrix<int> m1(1, 2, 9);
  Matrix<int> m2(10, 8, 7);
  Matrix<int> m3(1, 1, 5);
  Matrix<int> m4(2, 1, 4);
  m1 = m2 = m3 = m4;

  ASSERT_TRUE((m1 == m2) &&
              (m1 == m3) &&
              (m1 == m4) &&
              (m2 == m3) &&
              (m2 == m4) &&
              (m3 == m4));
}

TEST(AssignTest, SelfAssignment) {
  Matrix<int> m1(10, 10, 0);
  m1 = m1;

  ASSERT_TRUE(MatrixHasSize(m1, 10, 10));
  ASSERT_TRUE(MatrixIsFilledWith(m1, 0));
}
