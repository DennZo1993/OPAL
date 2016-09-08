#include "gtest/gtest.h"
#include "Matrix.h"

int RandomWithLimits(int lowerLimit, int upperLimit) {
  assert(upperLimit > lowerLimit);
  time_point<system_clock, microseconds> seed =
    time_point_cast<microseconds>(system_clock::now());
  srand(seed.time_since_epoch().count());
  int limitDiff = upperLimit - lowerLimit;
  return rand() % limitDiff + lowerLimit;
}

TEST(SSDTests, TestShiftRight) {
  int Height = 50, Width = 70;
  Matrix<double> src;
  src.AllocateRandomizedWithLimits(Height, Width, 0, 100);
  Matrix<double> dst;
  dst.AllocateRandomizedWithLimits(Height, Width, 10, 200);

  for (int patchSize = 1; patchSize < 10; ++patchSize) {
    int i1 = RandomWithLimits(patchSize + 1, Height - patchSize - 1);
    int j1 = RandomWithLimits(patchSize + 1, Width - patchSize - 1);
    int i2 = RandomWithLimits(patchSize + 1, Height - patchSize - 1);
    int j2 = RandomWithLimits(patchSize + 1, Width - patchSize - 1);

    auto originalSSD = src.SSD(dst, i1, j1, i2, j2, patchSize, patchSize);
    {
      auto shiftedSSD = src.SSD_ShiftedRight(dst, i1, j1, i2, j2, patchSize, patchSize, originalSSD);
      auto trustedSSD = src.SSD(dst, i1, j1 + 1, i2, j2 + 1, patchSize, patchSize);
      ASSERT_DOUBLE_EQ(trustedSSD, shiftedSSD);
    }
    {
      auto shiftedSSD = src.SSD_ShiftedLeft(dst, i1, j1, i2, j2, patchSize, patchSize, originalSSD);
      auto trustedSSD = src.SSD(dst, i1, j1 - 1, i2, j2 - 1, patchSize, patchSize);
      ASSERT_DOUBLE_EQ(trustedSSD, shiftedSSD);
    }
    {
      auto shiftedSSD = src.SSD_ShiftedUp(dst, i1, j1, i2, j2, patchSize, patchSize, originalSSD);
      auto trustedSSD = src.SSD(dst, i1 - 1, j1, i2 - 1, j2, patchSize, patchSize);
      ASSERT_DOUBLE_EQ(trustedSSD, shiftedSSD);
    }
    {
      auto shiftedSSD = src.SSD_ShiftedDown(dst, i1, j1, i2, j2, patchSize, patchSize, originalSSD);
      auto trustedSSD = src.SSD(dst, i1 + 1, j1, i2 + 1, j2, patchSize, patchSize);
      ASSERT_DOUBLE_EQ(trustedSSD, shiftedSSD);
    }
  }
}