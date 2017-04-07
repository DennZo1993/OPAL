#include "ImageDatabase.h"
#include "SSD.h"
#include "gtest/gtest.h"

// Death tests below check assertion failures in the code, so put them
// into #ifndef NDEBUG guards.


// Global image database (IBSR).
using DatabaseType = ImageDatabase<double, int>;
DatabaseType ibsr;


class SSDShiftTest : public ::testing::Test {
protected:
  using DatabaseType = ImageDatabase<int, int>;
  using TImage = typename DatabaseType::ImageType;

  virtual void SetUp() {
    db.Add(TImage(HEIGHT, WIDTH, 1), TImage(HEIGHT, WIDTH, 0)); // Fixed.
    db.Add(TImage(HEIGHT, WIDTH, 2), TImage(HEIGHT, WIDTH, 0)); // Moving.
    db.Add(TImage(HEIGHT, WIDTH, 3), TImage(HEIGHT, WIDTH, 0)); // Moving.
    db.Add(TImage(HEIGHT, WIDTH, 4), TImage(HEIGHT, WIDTH, 0)); // Moving.
  }

  virtual void TearDown() {}

  DatabaseType db;
  static constexpr size_t HEIGHT = 5;
  static constexpr size_t WIDTH = 5;
};


TEST_F(SSDShiftTest, ConstructionBad1) {
  auto creator = [this]() {
    SSD<DatabaseType> ssd(db, /*idx*/1, /*fix*/2, 2, /*mov*/1, 3, /*radius*/2);
  };

#ifndef NDEBUG
  ASSERT_DEATH(creator(), "Patch is outside moving image!");
#endif
}


TEST_F(SSDShiftTest, ShiftTop1) {
  SSD<DatabaseType> ssd(db, /*idx=*/ 1, /*fix*/2, 2,/*mov*/1, 3, /*radius=*/ 1);

  ASSERT_TRUE(ssd);
  ASSERT_EQ(9, ssd.GetValue());

  ssd.ShiftTop(); // Nowhere to shift, must be uncalculated.

  ASSERT_FALSE(ssd);
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

  ssd.ShiftTop(); // Nowhere to shift.
  ASSERT_FALSE(ssd);

#ifndef NDEBUG
  ASSERT_DEATH(ssd.ShiftBottom(), ".*"); // Cannot shift invalid SSD.
#endif
}


TEST_F(SSDShiftTest, ShiftBottom1) {
  SSD<DatabaseType> ssd(db, /*idx=*/ 3, /*fix*/2, 2,/*mov*/1, 3, /*radius=*/ 1);

  ASSERT_TRUE(ssd);
  ASSERT_EQ(81, ssd.GetValue());

  ssd.ShiftBottom(); // Nowhere to shift, must be uncalculated.

  ASSERT_FALSE(ssd);
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

  ssd.ShiftBottom(); // Nowhere to shift.
  ASSERT_FALSE(ssd);

#ifndef NDEBUG
  ASSERT_DEATH(ssd.ShiftTop(), ".*"); // Cannot shift invalid SSD.
#endif
}


TEST_F(SSDShiftTest, ShiftRight1) {
  SSD<DatabaseType> ssd(db, /*idx=*/ 1, /*fix*/2, 2,/*mov*/1, 1, /*radius=*/ 1);

  ASSERT_EQ(9, ssd.GetValue());

  ssd.ShiftRight();
  ASSERT_EQ(9, ssd.GetValue());

  ssd.ShiftRight();
  ASSERT_FALSE(ssd);
}
