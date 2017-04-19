#include "ImageDatabase.h"
#include "SSD.h"
#include "gtest/gtest.h"

// Death tests below check assertion failures in the code, so put them
// into #ifndef NDEBUG guards.


class SSDShiftTest : public ::testing::Test {
protected:
  using DatabaseType = ImageDatabase<int, int>;
  using TImage = typename DatabaseType::ImgType;

  static void SetUpTestCase() {
    db.Add(TImage(HEIGHT, WIDTH, 1), TImage(HEIGHT, WIDTH, 0)); // Fixed.
    db.Add(TImage(HEIGHT, WIDTH, 2), TImage(HEIGHT, WIDTH, 0)); // Moving.
    db.Add(TImage(HEIGHT, WIDTH, 3), TImage(HEIGHT, WIDTH, 0)); // Moving.
    db.Add(TImage(HEIGHT, WIDTH, 4), TImage(HEIGHT, WIDTH, 0)); // Moving.
  }

  virtual void SetUp() {}

  virtual void TearDown() {}

  static DatabaseType db;
  static constexpr size_t HEIGHT = 5;
  static constexpr size_t WIDTH = 5;
};


SSDShiftTest::DatabaseType SSDShiftTest::db;


TEST_F(SSDShiftTest, ShiftTop1) {
  SSD<DatabaseType> ssd(db, /*idx=*/ 1, /*fix*/2, 2,/*mov*/1, 3, /*radius=*/ 1);

  ASSERT_TRUE(ssd);
  ASSERT_EQ(9, ssd.GetValue());
}


TEST_F(SSDShiftTest, ShiftTop2) {
  SSD<DatabaseType> ssd(db, /*idx=*/ 3, /*fix*/2, 2,/*mov*/1, 3, /*radius=*/ 1);

  ASSERT_TRUE(ssd);
  ASSERT_EQ(81, ssd.GetValue());

  ssd.ShiftTop();
  ASSERT_TRUE(ssd);
  ASSERT_EQ(36, ssd.GetValue());

  ssd.ShiftTop();
  ASSERT_TRUE(ssd);
  ASSERT_EQ(9, ssd.GetValue());
}


TEST_F(SSDShiftTest, ShiftBottom1) {
  SSD<DatabaseType> ssd(db, /*idx=*/ 3, /*fix*/2, 2,/*mov*/1, 3, /*radius=*/ 1);

  ASSERT_TRUE(ssd);
  ASSERT_EQ(81, ssd.GetValue());
}


TEST_F(SSDShiftTest, ShiftBottom2) {
  SSD<DatabaseType> ssd(db, /*idx=*/ 1, /*fix*/2, 2,/*mov*/1, 3, /*radius=*/ 1);

  ASSERT_TRUE(ssd);
  ASSERT_EQ(9, ssd.GetValue());

  ssd.ShiftBottom();
  ASSERT_TRUE(ssd);
  ASSERT_EQ(36, ssd.GetValue());

  ssd.ShiftBottom();
  ASSERT_TRUE(ssd);
  ASSERT_EQ(81, ssd.GetValue());
}


TEST_F(SSDShiftTest, ShiftRight1) {
  SSD<DatabaseType> ssd(db, /*idx=*/ 1, /*fix*/2, 2,/*mov*/1, 1, /*radius=*/ 1);

  ASSERT_EQ(9, ssd.GetValue());

  ssd.ShiftRight();
  ASSERT_EQ(9, ssd.GetValue());
}


TEST_F(SSDShiftTest, ShiftRight2) {
  SSD<DatabaseType> ssd(db, /*idx=*/ 1, /*fix*/2, 3,/*mov*/1, 1, /*radius=*/ 1);

  ASSERT_EQ(9, ssd.GetValue());

  ssd.ShiftRight();
  ASSERT_EQ(9, ssd.GetValue());
}


TEST_F(SSDShiftTest, ShiftLeft1) {
  SSD<DatabaseType> ssd(db, /*idx=*/ 1, /*fix*/2, 2,/*mov*/2, 2, /*radius=*/ 2);

  ASSERT_EQ(25, ssd.GetValue());
}


class IBSRTest : public ::testing::Test {
protected:
  using DatabaseType = ImageDatabase<int, int>;

  static void SetUpTestCase() {
    ibsr.ReadFromConfig("test_data/IBSR.json");
  }

  virtual void SetUp() {}

  virtual void TearDown() {}

  static DatabaseType ibsr;
};

// 256 x 256 images.
IBSRTest::DatabaseType IBSRTest::ibsr;

// Tests below use some 'magic' numbers, which don't need to have these
// particular values. The intention is to perform multiple comparisons
// between 'truly calculated' SSD with values acquired after ShiftXXX at
// different coordinates and images.


TEST_F(IBSRTest, ShiftRight) {
  // Let us perform some runs...
  for (size_t run = 1; run < 10; ++run) {
    // ... for each run fix the row...
    size_t fixY = 10 + run * 10;
    size_t radius = run;
    // ... and select some columns...
    for (size_t fixX = radius+1; fixX < 200; fixX += 10) {
      size_t movX = fixX + 15;
      size_t movY = fixY + 25;

      // ... and perform the checks.
      SSD<DatabaseType> trueSSD(ibsr, run, fixX, fixY, movX, movY, radius);
      SSD<DatabaseType> shiftSSD(ibsr, run, fixX-1, fixY, movX-1, movY, radius);
      shiftSSD.ShiftRight();

      ASSERT_EQ(trueSSD.GetValue(), shiftSSD.GetValue());
    }
  }
}


TEST_F(IBSRTest, ShiftLeft) {
  // Let us perform some runs...
  for (size_t run = 1; run < 10; ++run) {
    // ... for each run fix the row...
    size_t fixY = 10 + run * 10;
    size_t radius = run;
    // ... and select some columns...
    for (size_t fixX = radius; fixX < 200; fixX += 10) {
      size_t movX = fixX + 15;
      size_t movY = fixY + 25;

      // ... and perform the checks.
      SSD<DatabaseType> trueSSD(ibsr, run, fixX, fixY, movX, movY, radius);
      SSD<DatabaseType> shiftSSD(ibsr, run, fixX+1, fixY, movX+1, movY, radius);
      shiftSSD.ShiftLeft();

      ASSERT_EQ(trueSSD.GetValue(), shiftSSD.GetValue());
    }
  }
}


TEST_F(IBSRTest, ShiftUp) {
  // Let us perform some runs...
  for (size_t run = 1; run < 10; ++run) {
    // ... for each run fix the row...
    size_t fixX = 10 + run * 10;
    size_t radius = run;
    // ... and select some columns...
    for (size_t fixY = radius; fixY < 200; fixY += 10) {
      size_t movX = fixX + 15;
      size_t movY = fixY + 25;

      // ... and perform the checks.
      SSD<DatabaseType> trueSSD(ibsr, run, fixX, fixY, movX, movY, radius);
      SSD<DatabaseType> shiftSSD(ibsr, run, fixX, fixY+1, movX, movY+1, radius);
      shiftSSD.ShiftUp();

      ASSERT_EQ(trueSSD.GetValue(), shiftSSD.GetValue());
    }
  }
}


TEST_F(IBSRTest, ShiftDown) {
  // Let us perform some runs...
  for (size_t run = 1; run < 10; ++run) {
    // ... for each run fix the row...
    size_t fixX = 10 + run * 10;
    size_t radius = run;
    // ... and select some columns...
    for (size_t fixY = radius+1; fixY < 200; fixY += 10) {
      size_t movX = fixX + 15;
      size_t movY = fixY + 25;

      // ... and perform the checks.
      SSD<DatabaseType> trueSSD(ibsr, run, fixX, fixY, movX, movY, radius);
      SSD<DatabaseType> shiftSSD(ibsr, run, fixX, fixY-1, movX, movY-1, radius);
      shiftSSD.ShiftDown();

      ASSERT_EQ(trueSSD.GetValue(), shiftSSD.GetValue());
    }
  }
}
