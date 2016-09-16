#include "Common.h"

TEST(RelationTest, TestEqualEmpty) {
  Matrix<int> m1;
  Matrix<int> m2(0, 1, -1);

  ASSERT_EQ(m1, m2);
  ASSERT_TRUE(m1 == m2);
  ASSERT_EQ(m2, m1);
  ASSERT_TRUE(m2 == m1);
}

TEST(RelationTest, TestEqualNonEmpty1) {
  Matrix<int> m1(3, 3, 1);
  Matrix<int> m2;
  m2.Allocate(3, 3);
  m2.Fill(1);

  ASSERT_EQ(m1, m2);
  ASSERT_TRUE(m1 == m2);
  ASSERT_EQ(m2, m1);
  ASSERT_TRUE(m2 == m1);
}

TEST(RelationTest, TestEqualNonEmpty2) {
  Matrix<int> m1(1, 4, 10);
  Matrix<float> m2(1, 4, 10.0);

  ASSERT_EQ(m2, m1.castTo<float>());
  ASSERT_EQ(m1, m2.castTo<int>());
}

TEST(RelationTest, TestEqualNonEmpty3) {
  Matrix<float> m1(1, 5, 4.0f);
  Matrix<float> m2(1, 5, 2.0f);

  ASSERT_EQ(m1, m2 + m2);
  ASSERT_EQ(m1 - m2, m2);
}

TEST(RelationTest, TestNotEqualEmptyNonEmpty) {
  Matrix<char> m1;
  Matrix<char> m2(1, 1, 'c');

  ASSERT_NE(m1, m2);
  ASSERT_TRUE(m1 != m2);
  ASSERT_FALSE(m1 == m2);

  ASSERT_NE(m2, m1);
  ASSERT_TRUE(m2 != m1);
  ASSERT_FALSE(m2 == m1);
}

TEST(RelationTest, TestNotEqualDifferentSize1) {
  Matrix<int> m1(1, 2, 0);
  Matrix<int> m2(2, 1, 0);

  ASSERT_NE(m1, m2);
  ASSERT_NE(m2, m1);
}

TEST(RelationTest, TestNotEqualDifferentSize2) {
  Matrix<int> m1(2, 3, 5);
  Matrix<int> m2(2, 4, 5);

  ASSERT_NE(m1, m2);
  ASSERT_NE(m2, m1);
}

TEST(RelationTest, TestNotEqualDifferentSize3) {
  Matrix<int> m1(3, 5, 8);
  Matrix<int> m2(1, 5, 8);

  ASSERT_NE(m1, m2);
  ASSERT_NE(m2, m1);
}

TEST(RelationTest, TestNotEqualSameSize) {
  Matrix<float> m1(3, 3, 1.0);
  Matrix<float> m2(3, 3, 1.0001);

  ASSERT_NE(m1, m2);
  ASSERT_NE(m2, m1);
}

TEST(RelationTest, TestSelf) {
  Matrix<double> m1;

  ASSERT_EQ(m1, m1);
}
