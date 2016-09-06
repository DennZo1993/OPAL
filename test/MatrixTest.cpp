#include "gtest/gtest.h"
#include "Matrix.h"
#include <fstream>

TEST(MatrixTests, TestReadFloat) {
  // Float-type matrix.
  Matrix<double> floatMat;
  std::ifstream ifs("matrix/3x4_ones_float.txt");
  ifs >> floatMat;

  ASSERT_EQ(3, floatMat.getHeight());
  ASSERT_EQ(4, floatMat.getWidth());
  for (int i = 0; i < floatMat.getHeight(); ++i) {
    for (int j = 0; j < floatMat.getWidth(); ++j) {
      ASSERT_DOUBLE_EQ(1.0, floatMat[i][j]);
    }
  }

  ifs.close();
}

TEST(MatrixTests, TestReadInt) {
  // Integer matrix.
  Matrix<int> intMat;
  std::ifstream ifs("matrix/3x4_ones_int.txt");
  ifs >> intMat;
  ASSERT_EQ(3, intMat.getHeight());
  ASSERT_EQ(4, intMat.getWidth());
  for (int i = 0; i < intMat.getHeight(); ++i) {
    for (int j = 0; j < intMat.getWidth(); ++j) {
      ASSERT_EQ(1, intMat[i][j]);
    }
  }

  ifs.close();
}

TEST(MatrixTests, TestReadAndSumFloat) {
  Matrix<double> ones;
  Matrix<double> twos;
  std::ifstream ifs1("matrix/3x4_ones_float.txt");
  std::ifstream ifs2("matrix/3x4_twos_float.txt");
  ifs1 >> ones;
  ifs2 >> twos;

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

  ifs1.close();
  ifs2.close();
}

TEST(MatrixTests, TestReadAndSumInt) {
  Matrix<int> ones;
  Matrix<int> twos;
  std::ifstream ifs1("matrix/3x4_ones_int.txt");
  std::ifstream ifs2("matrix/3x4_twos_int.txt");
  ifs1 >> ones;
  ifs2 >> twos;

  Matrix<int> threes = ones + twos;
  ASSERT_EQ(3, threes.getHeight());
  ASSERT_EQ(4, threes.getWidth());
  for (int i = 0; i < threes.getHeight(); ++i) {
    for (int j = 0; j < threes.getWidth(); ++j)
      ASSERT_EQ(3, threes[i][j]);
  }

  ones += twos;
  for (int i = 0; i < ones.getHeight(); ++i) {
    for (int j = 0; j < ones.getWidth(); ++j)
      ASSERT_EQ(3, ones[i][j]);
  }

  ASSERT_TRUE(ones == threes);
  ASSERT_TRUE(threes == ones);

  ifs1.close();
  ifs2.close();
}

TEST(MatrixTests, TestReadAndSubtractFloat) {
  Matrix<double> ones;
  Matrix<double> twos;
  std::ifstream ifs1("matrix/3x4_ones_float.txt");
  std::ifstream ifs2("matrix/3x4_twos_float.txt");
  ifs1 >> ones;
  ifs2 >> twos;

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

  ifs1.close();
  ifs2.close();
}

TEST(MatrixTests, TestReadAndSubtractInt) {
  Matrix<int> ones;
  Matrix<int> twos;
  std::ifstream ifs1("matrix/3x4_ones_int.txt");
  std::ifstream ifs2("matrix/3x4_twos_int.txt");
  ifs1 >> ones;
  ifs2 >> twos;

  Matrix<int> ones2 = twos - ones;
  ASSERT_EQ(3, ones2.getHeight());
  ASSERT_EQ(4, ones2.getWidth());
  for (int i = 0; i < ones2.getHeight(); ++i) {
    for (int j = 0; j < ones2.getWidth(); ++j)
      ASSERT_EQ(1, ones2[i][j]);
  }

  twos -= ones;
  for (int i = 0; i < twos.getHeight(); ++i) {
    for (int j = 0; j < twos.getWidth(); ++j)
      ASSERT_EQ(1, twos[i][j]);
  }

  ASSERT_TRUE(ones == twos);
  ASSERT_TRUE(twos == ones);

  ifs1.close();
  ifs2.close();
}