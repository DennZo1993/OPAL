#include "gtest/gtest.h"
#include "util/json/json.h"


using namespace util;


TEST(util, JsonFromFile1) {
  std::string fileName = "test_data/IBSR.json";
  auto json = JsonFromFile(fileName);

  ASSERT_FALSE(json.is_null());
}


TEST(util, JsonFromFile2) {
  const char *fileName = "test_data/IBSR.json";
  auto json = JsonFromFile(fileName);

  ASSERT_FALSE(json.is_null());
}


TEST(util, JsonFromFile3) {
  std::ifstream ifs("test_data/IBSR.json");
  auto json = JsonFromFile(ifs);

  ASSERT_FALSE(json.is_null());
}


TEST(util, JsonFromFileBad1) {
  ASSERT_THROW(JsonFromFile("bla"), JsonParseException);
  ASSERT_THROW(JsonFromFile("bla"), std::runtime_error);
}
