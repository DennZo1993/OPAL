#include "Common.h"

class AllocationTest : public ::testing::Test {
protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
  Matrix<int> m1;
};

TEST_F(AllocationTest, AllocationPositive) {
  m1.AllocateAndFill(10, 20, 5);
  ASSERT_FALSE(m1.isEmpty());
  ASSERT_TRUE(MatrixHasSize(m1, 10, 20));
  ASSERT_TRUE(MatrixIsFilledWith(m1, 5));
}

TEST_F(AllocationTest, AllocationZero1) {
  m1.AllocateAndFill(0, 10, 3);
  ASSERT_TRUE(MatrixIsEmpty(m1));
}

TEST_F(AllocationTest, AllocationZero2) {
  m1.AllocateAndFill(10, 0, 4);
  ASSERT_TRUE(MatrixIsEmpty(m1));
}

TEST_F(AllocationTest, AllocationZero3) {
  m1.AllocateAndFill(0, 0, 0);
  ASSERT_TRUE(MatrixIsEmpty(m1));
}

TEST_F(AllocationTest, AllocationNegative1) {
  ASSERT_THROW(m1.AllocateAndFill(-10, 10, 3),
               std::invalid_argument);
}

TEST_F(AllocationTest, AllocationNegative2) {
  ASSERT_THROW(m1.AllocateAndFill(10, -29, 4),
               std::invalid_argument);
}

TEST_F(AllocationTest, AllocationNegative3) {
  ASSERT_THROW(m1.AllocateAndFill(-1, -1, 0),
               std::invalid_argument);
}

class ReallocationTest : public ::testing::Test {
protected:
  virtual void SetUp() { m1.AllocateAndFill(5, 10, 2); }
  virtual void TearDown() {}
  Matrix<int> m1;
};

TEST_F(ReallocationTest, ReallocationToZero) {
  // Precondition - matrix 5x10 of 2
  ASSERT_TRUE(MatrixHasSize(m1, 5, 10));
  ASSERT_TRUE(MatrixIsFilledWith(m1, 2));

  m1.Allocate(0, 0);
  ASSERT_TRUE(MatrixIsEmpty(m1));
}

TEST_F(ReallocationTest, ReallocationToNegative) {
  // Precondition - matrix 5x10 of 2
  ASSERT_TRUE(MatrixHasSize(m1, 5, 10));
  ASSERT_TRUE(MatrixIsFilledWith(m1, 2));

  ASSERT_THROW(m1.Allocate(-1, -1),
               std::invalid_argument);
}

TEST_F(ReallocationTest, ReallocationGrow) {
  // Reallocation to a bigger matrix
  m1.Allocate(20, 30);
  ASSERT_TRUE(MatrixHasSize(m1, 20, 30));
}

TEST_F(ReallocationTest, ReallocationShrink) {
  // Reallocation to a bigger matrix
  m1.AllocateAndFill(1, 1, 5);
  ASSERT_TRUE(MatrixHasSize(m1, 1, 1));
  ASSERT_TRUE(MatrixIsFilledWith(m1, 5));
}

