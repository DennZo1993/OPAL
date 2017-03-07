#include "util.h"
#include "gtest/gtest.h"


TEST(util, FileToStringEmpty) {
  auto s = ReadFileToString("missing_file");
  ASSERT_EQ("", s);
}


TEST(util, FileToStringOneLine) {
  auto s = ReadFileToString("test_data/OneLine.txt");
  ASSERT_EQ("This is a one line file.\n", s);
}


TEST(util, FileToStringMultipleLines) {
  auto s = ReadFileToString("test_data/MultipleLines.txt");
  ASSERT_EQ("Line one.\nLine two.\n", s);
}
