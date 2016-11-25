#include "../Common.h"

class ImageReadToNonEmpty : public ::testing::Test {
protected:
  virtual void SetUp() { m1.Resize(10, 20, 3.5); }
  virtual void TearDown() {}
  Image<double> m1;
};

TEST_F(ImageReadToNonEmpty, ReadExistingNonEmptyText) {
  // Precondition.
  ASSERT_TRUE(ImageHasSize(m1, 10, 20));
  ASSERT_TRUE(ImageIsFilledWith(m1, 3.5));

  m1.ReadFromFile("test_data/matrix/3x4_twos_float.txt");
  ASSERT_TRUE(ImageHasSize(m1, 3, 4));
  ASSERT_TRUE(ImageIsFilledWith(m1, 2.0));
}

TEST_F(ImageReadToNonEmpty, ReadExistingNonEmptyTextStream) {
  // Precondition.
  ASSERT_TRUE(ImageHasSize(m1, 10, 20));
  ASSERT_TRUE(ImageIsFilledWith(m1, 3.5));

  std::ifstream ifs("test_data/matrix/3x4_twos_float.txt");
  ifs >> m1;
  ASSERT_TRUE(ImageHasSize(m1, 3, 4));
  ASSERT_TRUE(ImageIsFilledWith(m1, 2.0));
  ifs.close();
}

TEST_F(ImageReadToNonEmpty, ReadExistingEmptyText1) {
  // File with "0 0"
  m1.ReadFromFile("test_data/matrix/empty_0.txt");
  ASSERT_TRUE(ImageIsEmpty(m1));
}

TEST_F(ImageReadToNonEmpty, ReadExistingEmptyText2) {
  // File with "5"
  ASSERT_THROW(m1.ReadFromFile("test_data/matrix/empty_1.txt"),
               std::ifstream::failure);
  ASSERT_TRUE(ImageHasSize(m1, 10, 20));
  ASSERT_TRUE(ImageIsFilledWith(m1, 3.5));
}

TEST_F(ImageReadToNonEmpty, ReadExistingEmptyText3) {
  // Empty file
  ASSERT_THROW(m1.ReadFromFile("test_data/matrix/empty_2.txt"),
               std::ifstream::failure);
  ASSERT_TRUE(ImageHasSize(m1, 10, 20));
  ASSERT_TRUE(ImageIsFilledWith(m1, 3.5));
}

TEST_F(ImageReadToNonEmpty, ReadNonExisting) {
  // Non-existing file
  // Nothing should happen to the matrix.
  ASSERT_THROW(m1.ReadFromFile("test_data/matrix/qwasfkj.txt"),
               std::ifstream::failure);
  ASSERT_TRUE(ImageHasSize(m1, 10, 20));
  ASSERT_TRUE(ImageIsFilledWith(m1, 3.5));
}

TEST_F(ImageReadToNonEmpty, ReadSmallerText) {
  m1.ReadFromFile("test_data/matrix/3x4_real_2x3_ones_int.txt");
  ASSERT_TRUE(ImageHasSize(m1, 3, 4));
  // First 6 elements must be 1-s. The rest can be garbage.
  for (unsigned linearIdx = 0; linearIdx < 6; ++linearIdx) {
    int i = linearIdx / m1.getWidth();
    int j = linearIdx % m1.getWidth();
      ASSERT_DOUBLE_EQ(1.0, m1[i][j]);
  }
}

TEST_F(ImageReadToNonEmpty, ReadLargerText) {
  m1.ReadFromFile("test_data/matrix/3x4_real_5x5_ones_float.txt");
  ASSERT_TRUE(ImageHasSize(m1, 3, 4));
  ASSERT_TRUE(ImageIsFilledWith(m1, 1.0));
}

TEST_F(ImageReadToNonEmpty, ReadEmptyPNG) {
  ASSERT_THROW(m1.ReadFromPNG("test_data/pictures/empty.PNG"),
               std::runtime_error);
  ASSERT_TRUE(ImageHasSize(m1, 10, 20));
  ASSERT_TRUE(ImageIsFilledWith(m1, 3.5));
}

TEST_F(ImageReadToNonEmpty, ReadNonExistingPNG) {
  ASSERT_THROW(m1.ReadFromPNG("test_data/pictures/lkjaslkh.PNG"),
               std::runtime_error);
  ASSERT_TRUE(ImageHasSize(m1, 10, 20));
  ASSERT_TRUE(ImageIsFilledWith(m1, 3.5));
}

// More PNG reading test in ImageDatabase tests.

