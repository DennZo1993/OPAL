#include "gtest/gtest.h"
#include "Matrix.h"

int RandomWithLimits(int lowerLimit, int upperLimit) {
  assert(upperLimit > lowerLimit);
  using namespace std::chrono;
  time_point<system_clock, microseconds> seed =
    time_point_cast<microseconds>(system_clock::now());
  srand(seed.time_since_epoch().count());
  assert(upperLimit > lowerLimit);
  int limitDiff = upperLimit - lowerLimit;
  return rand() % limitDiff + lowerLimit;
}

template<class T>
void FillRandomizedWithLimits(Matrix<T> &m, int lowerLimit, int upperLimit) {
  for (unsigned i = 0; i < m.getHeight(); ++i)
    for (unsigned j = 0; j < m.getWidth(); ++j)
      m[i][j] = static_cast<T>(RandomWithLimits(lowerLimit, upperLimit));
}

class SSDShiftTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    Height = 50;
    Width = 70;

    src.Allocate(Height, Width);
    FillRandomizedWithLimits(src, 10, 100);

    dst.Allocate(Height, Width);
    FillRandomizedWithLimits(dst, 10, 200);
  }

  virtual void TearDown() {}

  Matrix<double> src;
  Matrix<double> dst;
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


