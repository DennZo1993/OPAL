#include "gtest/gtest.h"
#include "ImageIO/AnalyzeHeader.h"

#include <sstream>


using namespace Analyze;


TEST(AnalyzeHeader, ConstructionDefault) {
  // Check that static_assertions are fine.
  AnalyzeFileHeader hdr;
}


TEST(AnalyzeHeader, ConstructionFileOK) {
  FILE *f = fopen("test_data/02.hdr", "r");
  AnalyzeFileHeader hdr(f);
}


TEST(AnalyzeHeader, ConstructionFileBad) {
  auto create = [](const char *fileName) {
    FILE *f = fopen(fileName, "r");
    AnalyzeFileHeader hdr(f);
  };

  // No such file.
  ASSERT_THROW(create("bla"), std::runtime_error);

  // File doesn't contain Analyze header.
  ASSERT_THROW(create("test_data/OneLine.txt"), std::runtime_error);
}


TEST(AnalyzeHeader, PrintToStream) {
  // Just test we don't crash here since this printing is only for
  // debug purposes (so far) and human-readable.

  FILE *f = fopen("test_data/02.hdr", "r");
  AnalyzeFileHeader hdr(f);

  std::stringstream ss;
  ss << hdr;
}

