#include "gtest/gtest.h"
#include "util/fs/Directory.h"

#include <experimental/filesystem>


using namespace util;
namespace fs = std::experimental::filesystem;


TEST(util, ListDirSorted) {
  namespace fs = std::experimental::filesystem;
  auto files = ListDir("test_data/dir", /*ext=*/ "", /*sorted=*/ true);
  ASSERT_EQ(3, files.size());
  ASSERT_EQ("1.txt", fs::path(files[0]).filename());
  ASSERT_EQ("2.txt", fs::path(files[1]).filename());
  ASSERT_EQ("3.txt", fs::path(files[2]).filename());
}

