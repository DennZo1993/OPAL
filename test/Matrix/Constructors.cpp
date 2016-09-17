#include "Common.h"

Matrix<double> GetMatrix() {
  return Matrix<double>(5, 10, 7.0);
}

TEST(MatrixTest, Constructors0) {
  // Empty matrix
  // Read
  Matrix<double> m0("test_data/matrix/empty_0.txt");
  ASSERT_TRUE(MatrixIsEmpty(m0));
}

TEST(MatrixTest, Constructors1) {
  Matrix<double> m1;
  ASSERT_TRUE(MatrixIsEmpty(m1));
}

TEST(MatrixTest, Constructors2) {
  Matrix<int> m2(0, 20, 3);
  ASSERT_TRUE(MatrixIsEmpty(m2));
}

TEST(MatrixTest, Constructors3) {
  Matrix<int> m3(10, 0, 0);
  ASSERT_TRUE(MatrixIsEmpty(m3));
}

TEST(MatrixTest, Constructors4) {
  Matrix<int> m4(0, 0, 0);
  ASSERT_TRUE(MatrixIsEmpty(m4));
}

TEST(MatrixTest, Constructors5) {
  // Copy-construction from empty matrix
  Matrix<int> m2;
  Matrix<int> m5(m2);
  ASSERT_TRUE(MatrixIsEmpty(m5));
}

TEST(MatrixTest, Constructors6) {
  // Non-empty matrix
  Matrix<int> m6(5, 10, 2);
  ASSERT_FALSE(m6.isEmpty());
  ASSERT_TRUE(MatrixHasSize(m6, 5, 10));
  ASSERT_TRUE(MatrixIsFilledWith(m6, 2));
}

TEST(MatrixTest, Constructors7) {
  // Copy-construction from non-empty matrix
  Matrix<int> m6(5, 10, 2);
  Matrix<int> m7(m6);
  ASSERT_FALSE(m7.isEmpty());
  ASSERT_TRUE(MatrixHasSize(m7, 5, 10));
  ASSERT_TRUE(MatrixIsFilledWith(m7, 2));
}

TEST(MatrixTest, Constructors8) {
  // Read non-empty matrix
  Matrix<int> m8("test_data/matrix/3x4_ones_int.txt");
  ASSERT_FALSE(m8.isEmpty());
  ASSERT_TRUE(MatrixHasSize(m8, 3, 4));
  ASSERT_TRUE(MatrixIsFilledWith(m8, 1));
}

TEST(MatrixTest, Constructors9) {
  // Test move constructor
  Matrix<double> m9(GetMatrix());
  ASSERT_FALSE(m9.isEmpty());
  ASSERT_TRUE(MatrixHasSize(m9, 5, 10));
  ASSERT_TRUE(MatrixIsFilledWith(m9, 7.0));
}

TEST(MatrixTest, ConstructorsNegative1) {
  ASSERT_THROW(Matrix<int> m10(-1, 10, 5),
               std::invalid_argument);
}

TEST(MatrixTest, ConstructorsNegative2) {
  ASSERT_THROW(Matrix<int> m10(5, -1232, 5),
               std::invalid_argument);
}

TEST(MatrixTest, ConstructorsNegative3) {
  ASSERT_THROW(Matrix<int> m10(-1, -10, 5),
               std::invalid_argument);
}
