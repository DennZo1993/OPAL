#include "gtest/gtest.h"
#include "Matrix.h"

TEST(MatrixTests, ReadTest) {
  Matrix<double> m;
  m.ReadFromFile("matrix/3x4_ones_float.txt");

  ASSERT_EQ(3, m.getHeight());
  ASSERT_EQ(4, m.getWidth());
  for (int i = 0; i < m.getHeight(); ++i) {
    for (int j = 0; j < m.getWidth(); ++j) {
      ASSERT_DOUBLE_EQ(1.0, m[i][j]);
    }
  }
}
