#include "Common.h"

TEST(MatrixWrite, WriteEmpty) {
  std::string fileName = "test1.txt";
  std::ofstream ofs(fileName);

  Matrix<int> m1;
  ofs << m1;

  ofs.close();

  Matrix<int> m2(fileName);
  ASSERT_TRUE(MatrixIsEmpty(m2));
  ASSERT_EQ(m1, m2);
}

TEST(MatrixWrite, WriteNonEmpty1) {
  std::string fileName = "test2.txt";
  std::ofstream ofs(fileName);

  Matrix<int> m1(1, 10, 0);
  ofs << m1;

  ofs.close();

  Matrix<int> m2(fileName);
  ASSERT_EQ(m1, m2);
}

TEST(MatrixWrite, WriteNonEmpty2) {
  std::string fileName = "test3.txt";
  std::ofstream ofs(fileName);

  Matrix<float> m1(1, 1, 9.723);
  ofs << m1;

  ofs.close();

  Matrix<float> m2(fileName);
  ASSERT_EQ(m1, m2);
}

TEST(MatrixWrite, WriteNonEmpty3) {
  std::string fileName = "test4.txt";
  std::ofstream ofs(fileName);

  Matrix<double> m1(17, 11, 0.0001);
  ofs << m1;

  ofs.close();

  Matrix<double> m2(fileName);
  ASSERT_EQ(m1, m2);
}

TEST(MatrixWrite, WriteNonEmpty4) {
  std::string fileName = "test5.txt";
  std::ofstream ofs(fileName);

  Matrix<char> m1(2, 2, 't');
  ofs << m1;

  ofs.close();

  Matrix<char> m2(fileName);
  ASSERT_EQ(m1, m2);
}

TEST(MatrixWrite, WriteNonEmpty5) {
  std::string fileName = "test6.txt";
  std::ofstream ofs(fileName);

  Matrix<int> m1;
  m1.Allocate(500, 400);
  FillRandomizedWithLimits(m1, 15, 190);
  ofs << m1;

  ofs.close();

  Matrix<int> m2(fileName);
  ASSERT_EQ(m1, m2);
}

