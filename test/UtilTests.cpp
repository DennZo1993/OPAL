#include "util.h"
#include "gtest/gtest.h"
#include <experimental/filesystem>


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


TEST(util, ListDirSorted) {
  namespace fs = std::experimental::filesystem;
  auto files = ListDir("test_data/dir", /*sorted=*/ true);
  ASSERT_EQ(3, files.size());
  ASSERT_EQ("1.txt", fs::path(files[0]).filename());
  ASSERT_EQ("2.txt", fs::path(files[1]).filename());
  ASSERT_EQ("3.txt", fs::path(files[2]).filename());
}
