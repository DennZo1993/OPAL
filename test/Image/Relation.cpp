#include "../Common.h"

TEST(RelationTest, TestEqualEmpty) {
  Image<int> m1;
  Image<int> m2(0, 1, -1);

  ASSERT_EQ(m1, m2);
  ASSERT_TRUE(m1 == m2);
  ASSERT_EQ(m2, m1);
  ASSERT_TRUE(m2 == m1);
}

TEST(RelationTest, TestEqualNonEmpty1) {
  Image<int> m1(3, 3, 1);
  Image<int> m2;
  m2.Resize(3, 3);
  m2.Fill(1);

  ASSERT_EQ(m1, m2);
  ASSERT_TRUE(m1 == m2);
  ASSERT_EQ(m2, m1);
  ASSERT_TRUE(m2 == m1);
}

TEST(RelationTest, TestEqualNonEmpty2) {
  Image<int> m1(1, 4, 10);
  Image<float> m2(1, 4, 10.0);

  ASSERT_EQ(m2, m1.castTo<float>());
  ASSERT_EQ(m1, m2.castTo<int>());
}

TEST(RelationTest, TestEqualFloat) {
  // Equal for float.

  Image<float> m1(1, 5, 4.00000021);
  Image<float> m2(1, 5, 2.000000104);

  ASSERT_EQ(m1, m2 + m2);
  ASSERT_EQ(m1 - m2, m2);
}

TEST(RelationTest, TestiNotEqualDouble) {
  // NOT equal for double.

  Image<double> m1(1, 5, 4.00000021);
  Image<double> m2(1, 5, 2.000000104);

  ASSERT_NE(m1, m2 + m2);
  ASSERT_NE(m1 - m2, m2);
}

TEST(RelationTest, TestNotEqualEmptyNonEmpty) {
  Image<char> m1;
  Image<char> m2(1, 1, 'c');

  ASSERT_NE(m1, m2);
  ASSERT_TRUE(m1 != m2);
  ASSERT_FALSE(m1 == m2);

  ASSERT_NE(m2, m1);
  ASSERT_TRUE(m2 != m1);
  ASSERT_FALSE(m2 == m1);
}

TEST(RelationTest, TestNotEqualDifferentSize1) {
  Image<int> m1(1, 2, 0);
  Image<int> m2(2, 1, 0);

  ASSERT_NE(m1, m2);
  ASSERT_NE(m2, m1);
}

TEST(RelationTest, TestNotEqualDifferentSize2) {
  Image<int> m1(2, 3, 5);
  Image<int> m2(2, 4, 5);

  ASSERT_NE(m1, m2);
  ASSERT_NE(m2, m1);
}

TEST(RelationTest, TestNotEqualDifferentSize3) {
  Image<int> m1(3, 5, 8);
  Image<int> m2(1, 5, 8);

  ASSERT_NE(m1, m2);
  ASSERT_NE(m2, m1);
}

TEST(RelationTest, TestNotEqualSameSize) {
  Image<float> m1(3, 3, 1.0);
  Image<float> m2(3, 3, 1.0001);

  ASSERT_NE(m1, m2);
  ASSERT_NE(m2, m1);
}

TEST(RelationTest, TestSelf) {
  Image<double> m1;

  ASSERT_EQ(m1, m1);
}
