#include "Common.h"

class MatrixReadToNonEmpty : public ::testing::Test {
protected:
  virtual void SetUp() { m1.AllocateAndFill(10, 20, 3.5); }
  virtual void TearDown() {}
  Matrix<double> m1;
};

TEST_F(MatrixReadToNonEmpty, ReadExistingNonEmptyText) {
  // Precondition.
  ASSERT_TRUE(MatrixHasSize(m1, 10, 20));
  ASSERT_TRUE(MatrixIsFilledWith(m1, 3.5));

  m1.ReadFromFile("test_data/matrix/3x4_twos_float.txt");
  ASSERT_TRUE(MatrixHasSize(m1, 3, 4));
  ASSERT_TRUE(MatrixIsFilledWith(m1, 2.0));
}

TEST_F(MatrixReadToNonEmpty, ReadExistingNonEmptyTextStream) {
  // Precondition.
  ASSERT_TRUE(MatrixHasSize(m1, 10, 20));
  ASSERT_TRUE(MatrixIsFilledWith(m1, 3.5));

  std::ifstream ifs("test_data/matrix/3x4_twos_float.txt");
  ifs >> m1;
  ASSERT_TRUE(MatrixHasSize(m1, 3, 4));
  ASSERT_TRUE(MatrixIsFilledWith(m1, 2.0));
  ifs.close();
}

TEST_F(MatrixReadToNonEmpty, ReadExistingEmptyText1) {
  // File with "0 0"
  m1.ReadFromFile("test_data/matrix/empty_0.txt");
  ASSERT_TRUE(MatrixIsEmpty(m1));
}

TEST_F(MatrixReadToNonEmpty, ReadExistingEmptyText2) {
  // File with "5"
  m1.ReadFromFile("test_data/matrix/empty_1.txt");
  ASSERT_TRUE(MatrixIsEmpty(m1));
}

TEST_F(MatrixReadToNonEmpty, ReadExistingEmptyText3) {
  // Empty file
  m1.ReadFromFile("test_data/matrix/empty_2.txt");
  ASSERT_TRUE(MatrixIsEmpty(m1));
}

TEST_F(MatrixReadToNonEmpty, ReadNonExisting) {
  // Non-existing file
  // Nothing shoul happen to the matrix.
  m1.ReadFromFile("test_data/matrix/qwasfkj.txt");
  ASSERT_TRUE(MatrixHasSize(m1, 10, 20));
  ASSERT_TRUE(MatrixIsFilledWith(m1, 3.5));
}

TEST_F(MatrixReadToNonEmpty, ReadSmallerText) {
  m1.ReadFromFile("test_data/matrix/3x4_real_2x3_ones_int.txt");
  ASSERT_TRUE(MatrixHasSize(m1, 3, 4));
  // First 6 elements must be 1-s. The rest can be garbage.
  for (unsigned linearIdx = 0; linearIdx < 6; ++linearIdx) {
    int i = linearIdx / m1.getWidth();
    int j = linearIdx % m1.getWidth();
      ASSERT_DOUBLE_EQ(1.0, m1[i][j]);
  }
}

TEST_F(MatrixReadToNonEmpty, ReadLargerText) {
  m1.ReadFromFile("test_data/matrix/3x4_real_5x5_ones_float.txt");
  ASSERT_TRUE(MatrixHasSize(m1, 3, 4));
  ASSERT_TRUE(MatrixIsFilledWith(m1, 1.0));
}

TEST_F(MatrixReadToNonEmpty, ReadEmptyPNG) {
  m1.ReadFromPNG("test_data/pictures/empty.PNG");
  ASSERT_TRUE(MatrixHasSize(m1, 10, 20));
  ASSERT_TRUE(MatrixIsFilledWith(m1, 3.5));
}

TEST_F(MatrixReadToNonEmpty, ReadNonExistingPNG) {
  m1.ReadFromPNG("test_data/pictures/lkjaslkh.PNG");
  ASSERT_TRUE(MatrixHasSize(m1, 10, 20));
  ASSERT_TRUE(MatrixIsFilledWith(m1, 3.5));
}

// More PNG reading test in ImageDatabase tests.

