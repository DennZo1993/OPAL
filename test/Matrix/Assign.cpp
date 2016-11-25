#include "../Common.h"

TEST(AssignTest, AssignEmptyToEmpty) {
  Image<unsigned> m1;
  Image<unsigned> m2;
  m1 = m2;

  ASSERT_TRUE(ImageIsEmpty(m1));
  ASSERT_TRUE(ImageIsEmpty(m2));
}

TEST(AssignTest, AssignEmptyToNonEmpty1) {
  Image<int> m1(10, 10, 9);
  Image<int> m2;
  m1 = m2;

  ASSERT_TRUE(ImageIsEmpty(m1));
  ASSERT_TRUE(ImageIsEmpty(m2));
}

TEST(AssignTest, AssignEmptyToNonEmpty2) {
  Image<int> m1(10, 10, 9);
  Image<int> m2(2, 4, 7);
  Image<int> m3;
  m1 = m2 = m3;

  ASSERT_TRUE(ImageIsEmpty(m1));
  ASSERT_TRUE(ImageIsEmpty(m2));
  ASSERT_TRUE(ImageIsEmpty(m3));
}

TEST(AssignTest, AssignNonEmptyToEmpty) {
  Image<double> m1;
  Image<double> m2(1, 1, 1.0);
  m1 = m2;

  ASSERT_FALSE(m1.isEmpty());
  ASSERT_TRUE(ImageHasSize(m1, 1, 1));
  ASSERT_TRUE(ImageIsFilledWith(m1, 1.0));
}

TEST(AssignTest, AssignNonEmpties) {
  Image<int> m1(1, 2, 9);
  Image<int> m2(10, 8, 7);
  Image<int> m3(1, 1, 5);
  Image<int> m4(2, 1, 4);
  m1 = m2 = m3 = m4;

  ASSERT_TRUE((m1 == m2) &&
              (m1 == m3) &&
              (m1 == m4) &&
              (m2 == m3) &&
              (m2 == m4) &&
              (m3 == m4));
}

TEST(AssignTest, SelfAssignment) {
  Image<int> m1(10, 10, 0);
  m1 = m1;

  ASSERT_TRUE(ImageHasSize(m1, 10, 10));
  ASSERT_TRUE(ImageIsFilledWith(m1, 0));
}
