#include "../Common.h"

class AllocationTest : public ::testing::Test {
protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
  Image<int> m1;
};

TEST_F(AllocationTest, AllocationPositive) {
  m1.Resize(10, 20, 5);
  ASSERT_FALSE(m1.isEmpty());
  ASSERT_TRUE(ImageHasSize(m1, 10, 20));
  ASSERT_TRUE(ImageIsFilledWith(m1, 5));
}

TEST_F(AllocationTest, AllocationZero1) {
  m1.Resize(0, 10, 3);
  ASSERT_TRUE(ImageIsEmpty(m1));
}

TEST_F(AllocationTest, AllocationZero2) {
  m1.Resize(10, 0, 4);
  ASSERT_TRUE(ImageIsEmpty(m1));
}

TEST_F(AllocationTest, AllocationZero3) {
  m1.Resize(0, 0, 0);
  ASSERT_TRUE(ImageIsEmpty(m1));
}

TEST_F(AllocationTest, AllocationNegative1) {
  ASSERT_ANY_THROW(m1.Resize(-10, 10, 3));
}

TEST_F(AllocationTest, AllocationNegative2) {
  ASSERT_ANY_THROW(m1.Resize(10, -29, 4));
}


class ReallocationTest : public ::testing::Test {
protected:
  virtual void SetUp() { m1.Resize(5, 10, 2); }
  virtual void TearDown() {}
  Image<int> m1;
};

TEST_F(ReallocationTest, ReallocationToZero) {
  // Precondition - matrix 5x10 of 2
  ASSERT_TRUE(ImageHasSize(m1, 5, 10));
  ASSERT_TRUE(ImageIsFilledWith(m1, 2));

  m1.Resize(0, 0);
  ASSERT_TRUE(ImageIsEmpty(m1));
}

TEST_F(ReallocationTest, ReallocationGrow) {
  // Reallocation to a bigger matrix
  m1.Resize(20, 30);
  ASSERT_TRUE(ImageHasSize(m1, 20, 30));
}

TEST_F(ReallocationTest, ReallocationShrink) {
  // Reallocation to a bigger matrix
  m1.Resize(1, 1, 5);
  ASSERT_TRUE(ImageHasSize(m1, 1, 1));
  ASSERT_TRUE(ImageIsFilledWith(m1, 2));
}

