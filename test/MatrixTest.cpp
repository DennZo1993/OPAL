#include "gtest/gtest.h"
#include "Matrix.h"
#include <fstream>

// =====
// Allocation tests.
// =====
TEST(MatrixTests, TestAllocationViaConstructor) {
  {
    // Empty matrix.
    Matrix<double> m1;
    ASSERT_TRUE(m1.isEmpty());
    ASSERT_EQ(0, m1.getHeight());
    ASSERT_EQ(0, m1.getWidth());
    m1.AllocateAndFill(20, 10, 5.0);
    ASSERT_FALSE(m1.isEmpty());
    ASSERT_EQ(20, m1.getHeight());
    ASSERT_EQ(10, m1.getWidth());
  }
  {
    // Non-empty matrix via constructor.
    Matrix<int> m1(5, 10, 2);
    ASSERT_FALSE(m1.isEmpty());
    ASSERT_EQ(5, m1.getHeight());
    ASSERT_EQ(10, m1.getWidth());

    Matrix<int> m2(m1);
    ASSERT_FALSE(m2.isEmpty());
    ASSERT_EQ(5, m2.getHeight());
    ASSERT_EQ(10, m2.getWidth());
  }
  {
    // Empty matrix via constructor.
    Matrix<double> m1;
    ASSERT_TRUE(m1.isEmpty());
    ASSERT_EQ(0, m1.getHeight());
    ASSERT_EQ(0, m1.getWidth());
    Matrix<double> m2(m1);
    ASSERT_TRUE(m2.isEmpty());
    ASSERT_EQ(0, m2.getHeight());
    ASSERT_EQ(0, m2.getWidth());
  }
}

TEST(MatrixTests, TestReallocation) {
  {
    // Test explicit reallocation - shrink.
    Matrix<int> m1(5, 10, 2);
    ASSERT_FALSE(m1.isEmpty());
    ASSERT_EQ(5, m1.getHeight());
    ASSERT_EQ(10, m1.getWidth());
    m1.AllocateAndFill(1, 1, 5);
    ASSERT_FALSE(m1.isEmpty());
    ASSERT_EQ(1, m1.getHeight());
    ASSERT_EQ(1, m1.getWidth());
  }
  {
    // Test reallocation to empty.
    Matrix<int> m1(5, 10, 2);
    ASSERT_FALSE(m1.isEmpty());
    ASSERT_EQ(5, m1.getHeight());
    ASSERT_EQ(10, m1.getWidth());
    m1.AllocateAndFill(20, 0, 4);
    ASSERT_TRUE(m1.isEmpty());
    ASSERT_EQ(0, m1.getHeight());
    ASSERT_EQ(0, m1.getWidth());

    Matrix<int> m2(5, 10, 2);
    ASSERT_FALSE(m2.isEmpty());
    ASSERT_EQ(5, m2.getHeight());
    ASSERT_EQ(10, m2.getWidth());
    m2.AllocateAndFill(0, 40, 1);
    ASSERT_TRUE(m2.isEmpty());
    ASSERT_EQ(0, m2.getHeight());
    ASSERT_EQ(0, m2.getWidth());

    Matrix<int> m3(5, 10, 2);
    ASSERT_FALSE(m3.isEmpty());
    ASSERT_EQ(5, m3.getHeight());
    ASSERT_EQ(10, m3.getWidth());
    m3.AllocateAndFill(0, 0, 4);
    ASSERT_TRUE(m3.isEmpty());
    ASSERT_EQ(0, m3.getHeight());
    ASSERT_EQ(0, m3.getWidth());
  }
  {
    // Test explicit reallocation - grow.
    Matrix<int> m1(1, 1, 2);
    ASSERT_FALSE(m1.isEmpty());
    ASSERT_EQ(1, m1.getHeight());
    ASSERT_EQ(1, m1.getWidth());
    m1.AllocateAndFill(10, 20, 5);
    ASSERT_FALSE(m1.isEmpty());
    ASSERT_EQ(10, m1.getHeight());
    ASSERT_EQ(20, m1.getWidth());
  }
  {
    // Test reallocation via assignment - grow.
    Matrix<double> m1(2, 3, 5.0);
    ASSERT_FALSE(m1.isEmpty());
    ASSERT_EQ(2, m1.getHeight());
    ASSERT_EQ(3, m1.getWidth());
    Matrix<double> m2(15, 30, 10.0);
    ASSERT_FALSE(m2.isEmpty());
    ASSERT_EQ(15, m2.getHeight());
    ASSERT_EQ(30, m2.getWidth());
    m1 = m2;
    ASSERT_FALSE(m1.isEmpty());
    ASSERT_EQ(15, m1.getHeight());
    ASSERT_EQ(30, m1.getWidth());
    for (int i = 0; i < m1.getHeight(); ++i)
      for (int j = 0; j < m1.getWidth(); ++j)
        ASSERT_DOUBLE_EQ(10.0, m1[i][j]);
  }
  {
    // Test reallocation via assignment - shrink.
    Matrix<double> m1(2, 3, 5.0);
    ASSERT_FALSE(m1.isEmpty());
    ASSERT_EQ(2, m1.getHeight());
    ASSERT_EQ(3, m1.getWidth());
    Matrix<double> m2;
    m1 = m2;
    ASSERT_TRUE(m1.isEmpty());
    ASSERT_EQ(0, m1.getHeight());
    ASSERT_EQ(0, m1.getWidth());
  }
}

TEST(MatrixTests, TestCastTo) {
  {
    // Test casting non-empty matrix. Floor.
    Matrix<double> m1(2, 3, 1.3);
    Matrix<int> m2 = m1.castTo<int>();
    ASSERT_FALSE(m2.isEmpty());
    ASSERT_EQ(2, m2.getHeight());
    ASSERT_EQ(3, m2.getWidth());
    for (int i = 0; i < m2.getHeight(); ++i)
      for (int j = 0; j < m2.getWidth(); ++j)
        ASSERT_EQ(1, m2[i][j]);
  }
  {
    // Test casting empty matrix.
    Matrix<double> m1;
    Matrix<int> m2 = m1.castTo<int>();
    ASSERT_TRUE(m2.isEmpty());
    ASSERT_EQ(0, m2.getHeight());
    ASSERT_EQ(0, m2.getWidth());
  }
}

// =====
// Read + arithmetics tests.
// =====

TEST(MatrixTests, TestReadFloat) {
  // Float-type matrix.
  Matrix<double> floatMat;
  std::ifstream ifs("test_data/matrix/3x4_ones_float.txt");
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
  std::ifstream ifs("test_data/matrix/3x4_ones_int.txt");
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
  std::ifstream ifs1("test_data/matrix/3x4_ones_float.txt");
  std::ifstream ifs2("test_data/matrix/3x4_twos_float.txt");
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
  std::ifstream ifs1("test_data/matrix/3x4_ones_int.txt");
  std::ifstream ifs2("test_data/matrix/3x4_twos_int.txt");
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
  std::ifstream ifs1("test_data/matrix/3x4_ones_float.txt");
  std::ifstream ifs2("test_data/matrix/3x4_twos_float.txt");
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
  std::ifstream ifs1("test_data/matrix/3x4_ones_int.txt");
  std::ifstream ifs2("test_data/matrix/3x4_twos_int.txt");
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