#include "Common.h"

class SSDShiftTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    Height = 50;
    Width = 70;

    src.Allocate(Height, Width);
    FillRandomizedWithLimits(src, 10, 100);

    dst.Allocate(Height, Width);
    FillRandomizedWithLimits(dst, 10, 200);

    small1.AllocateAndFill(3, 3, 1.0);
    small2.AllocateAndFill(3, 3, 2.0);
  }

  virtual void TearDown() {}

  Matrix<double> src;
  Matrix<double> dst;
  Matrix<double> small1;
  Matrix<double> small2;

  unsigned Height;
  unsigned Width;
};


TEST_F(SSDShiftTest, TestShiftRight) {
  for (int patchSize = 1; patchSize < 10; ++patchSize) {
    int i1 = RandomWithLimits(patchSize + 1, Height - patchSize - 1);
    int j1 = RandomWithLimits(patchSize + 1, Width - patchSize - 1);
    int i2 = RandomWithLimits(patchSize + 1, Height - patchSize - 1);
    int j2 = RandomWithLimits(patchSize + 1, Width - patchSize - 1);

    auto originalSSD = src.SSD(dst, i1, j1, i2, j2, patchSize, patchSize);
    auto shiftedSSD = src.SSD_ShiftedRight(dst, i1, j1, i2, j2, patchSize,
                                           patchSize, originalSSD);
    auto trustedSSD = src.SSD(dst, i1, j1 + 1, i2, j2 + 1, patchSize, patchSize);
    ASSERT_DOUBLE_EQ(trustedSSD, shiftedSSD);
  }
}


TEST_F(SSDShiftTest, TestShiftLeft) {
  for (int patchSize = 1; patchSize < 10; ++patchSize) {
    int i1 = RandomWithLimits(patchSize + 1, Height - patchSize - 1);
    int j1 = RandomWithLimits(patchSize + 1, Width - patchSize - 1);
    int i2 = RandomWithLimits(patchSize + 1, Height - patchSize - 1);
    int j2 = RandomWithLimits(patchSize + 1, Width - patchSize - 1);

    auto originalSSD = src.SSD(dst, i1, j1, i2, j2, patchSize, patchSize);
    auto shiftedSSD = src.SSD_ShiftedLeft(dst, i1, j1, i2, j2, patchSize,
                                          patchSize, originalSSD);
    auto trustedSSD =
      src.SSD(dst, i1, j1 - 1, i2, j2 - 1, patchSize, patchSize);
    ASSERT_DOUBLE_EQ(trustedSSD, shiftedSSD);
  }
}


TEST_F(SSDShiftTest, TestShiftUp) {
  for (int patchSize = 1; patchSize < 10; ++patchSize) {
    int i1 = RandomWithLimits(patchSize + 1, Height - patchSize - 1);
    int j1 = RandomWithLimits(patchSize + 1, Width - patchSize - 1);
    int i2 = RandomWithLimits(patchSize + 1, Height - patchSize - 1);
    int j2 = RandomWithLimits(patchSize + 1, Width - patchSize - 1);

    auto originalSSD = src.SSD(dst, i1, j1, i2, j2, patchSize, patchSize);
    auto shiftedSSD = src.SSD_ShiftedUp(dst, i1, j1, i2, j2, patchSize,
                                        patchSize, originalSSD);
    auto trustedSSD =
      src.SSD(dst, i1 - 1, j1, i2 - 1, j2, patchSize, patchSize);
    ASSERT_DOUBLE_EQ(trustedSSD, shiftedSSD);
  }
}


TEST_F(SSDShiftTest, TestShiftDown) {
  for (int patchSize = 1; patchSize < 10; ++patchSize) {
    int i1 = RandomWithLimits(patchSize + 1, Height - patchSize - 1);
    int j1 = RandomWithLimits(patchSize + 1, Width - patchSize - 1);
    int i2 = RandomWithLimits(patchSize + 1, Height - patchSize - 1);
    int j2 = RandomWithLimits(patchSize + 1, Width - patchSize - 1);

    auto originalSSD = src.SSD(dst, i1, j1, i2, j2, patchSize, patchSize);
    auto shiftedSSD = src.SSD_ShiftedDown(dst, i1, j1, i2, j2, patchSize,
                                           patchSize, originalSSD);
    auto trustedSSD =
      src.SSD(dst, i1 + 1, j1, i2 + 1, j2, patchSize, patchSize);
    ASSERT_DOUBLE_EQ(trustedSSD, shiftedSSD);
  }
}


TEST_F(SSDShiftTest, TestSSDFail1) {
  // Incorrect patch sizes.
  ASSERT_THROW(small1.SSD(small2, 1, 1, 1, 1, 0, 1), std::invalid_argument);
  ASSERT_THROW(small1.SSD(small2, 1, 1, 1, 1, 1, 0), std::invalid_argument);
  ASSERT_THROW(small1.SSD(small2, 1, 1, 1, 1, 2, 1), std::invalid_argument);
  ASSERT_THROW(small1.SSD(small2, 1, 1, 1, 1, 1, 5), std::invalid_argument);
}


TEST_F(SSDShiftTest, TestShiftFail1) {
  auto orig = small1.SSD(small2, 1, 1, 1, 1, 1, 1);
  ASSERT_DOUBLE_EQ(9.0, orig);

  ASSERT_THROW(small1.SSD_ShiftedRight(small2, 1, 1, 1, 1, 1, 1, orig),
               std::invalid_argument);
  ASSERT_THROW(small1.SSD_ShiftedLeft(small2, 1, 1, 1, 1, 1, 1, orig),
               std::invalid_argument);
  ASSERT_THROW(small1.SSD_ShiftedUp(small2, 1, 1, 1, 1, 1, 1, orig),
               std::invalid_argument);
  ASSERT_THROW(small1.SSD_ShiftedDown(small2, 1, 1, 1, 1, 1, 1, orig),
               std::invalid_argument);
}


TEST(SSDWholeMatrix, TestEmpty) {
  Matrix<int> m1;
  Matrix<int> m2;

  ASSERT_EQ(0, m1.SSD(m2));
  ASSERT_EQ(0, m2.SSD(m1));
}


TEST(SSDWholeMatrix, TestSameSize1) {
  Matrix<double> m1(1, 1, 2.1);
  Matrix<double> m2(1, 1, 0.98);

  ASSERT_DOUBLE_EQ(1.2544, m1.SSD(m2));
  ASSERT_DOUBLE_EQ(1.2544, m2.SSD(m1));
}


TEST(SSDWholeMatrix, TestSameSize2) {
  Matrix<double> m1;
  m1.Allocate(100, 30);
  FillRandomizedWithLimits(m1, 0, 100);
  Matrix<double> m2;
  m2.Allocate(100, 30);
  FillRandomizedWithLimits(m2, 0, 100);

  ASSERT_DOUBLE_EQ(m1.SSD(m2), m2.SSD(m1));
}


TEST(SSDWholeMatrix, TestDifferentSize1) {
  Matrix<int> m1;
  Matrix<int> m2(1, 1, 0);

  ASSERT_THROW(m1.SSD(m2), std::invalid_argument);
  ASSERT_THROW(m2.SSD(m1), std::invalid_argument);
}


TEST(SSDWholeMatrix, TestDifferentSize2) {
  Matrix<int> m1(2, 4, 9);
  Matrix<int> m2(2, 5, 0);

  ASSERT_THROW(m1.SSD(m2), std::invalid_argument);
  ASSERT_THROW(m2.SSD(m1), std::invalid_argument);
}
