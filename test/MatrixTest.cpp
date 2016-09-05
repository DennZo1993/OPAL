#include "gtest/gtest.h"
#include "Matrix.h"

TEST(MatrixTests, TestRead) {
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

TEST(MatrixTests, TestReadAndSum) {
  Matrix<double> ones;
  Matrix<double> twos;
  ones.ReadFromFile("matrix/3x4_ones_float.txt");
  twos.ReadFromFile("matrix/3x4_twos_float.txt");

  Matrix<double> threes = ones + twos;
  ASSERT_EQ(3, threes.getHeight());
  ASSERT_EQ(4, threes.getWidth());
  for (int i = 0; i < threes.getHeight(); ++i) {
    for (int j = 0; j < threes.getWidth(); ++j)
      ASSERT_DOUBLE_EQ(3.0, threes[i][j]);
  }

  ones += twos;
  for (int i = 0; i < ones.getHeight(); ++i) {
    for (int j = 0; j < ones.getWidth(); ++j)
      ASSERT_DOUBLE_EQ(3.0, ones[i][j]);
  }

  ASSERT_TRUE(ones == threes);
  ASSERT_TRUE(threes == ones);
}

TEST(MatrixTests, TestReadAndSubtract) {
  Matrix<double> ones;
  Matrix<double> twos;
  ones.ReadFromFile("matrix/3x4_ones_float.txt");
  twos.ReadFromFile("matrix/3x4_twos_float.txt");

  Matrix<double> ones2 = twos - ones;
  ASSERT_EQ(3, ones2.getHeight());
  ASSERT_EQ(4, ones2.getWidth());
  for (int i = 0; i < ones2.getHeight(); ++i) {
    for (int j = 0; j < ones2.getWidth(); ++j)
      ASSERT_DOUBLE_EQ(1.0, ones2[i][j]);
  }

  twos -= ones;
  for (int i = 0; i < twos.getHeight(); ++i) {
    for (int j = 0; j < twos.getWidth(); ++j)
      ASSERT_DOUBLE_EQ(1.0, twos[i][j]);
  }

  ASSERT_TRUE(ones == twos);
  ASSERT_TRUE(twos == ones);
}
