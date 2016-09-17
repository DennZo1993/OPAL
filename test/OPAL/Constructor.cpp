#include "OPAL.h"
#include "gtest/gtest.h"

TEST(OPAL, ConsturctorFromEmptyDB) {
  OPAL::DatabaseType db;

  ASSERT_THROW(OPAL opal(db), std::logic_error);
}

TEST(OPAL, ConstructorFromDBWithOnePair) {
  OPAL::DatabaseType db;
  db.Add("test_data/pictures/alley_1_frame_0001.png",
         "test_data/pictures/alley_1_frame_0002.png");

  ASSERT_THROW(OPAL opal(db), std::logic_error);
}

TEST(OPAL, ConstructorFromGoodDB) {
  OPAL::DatabaseType db;
  db.Add("test_data/pictures/alley_1_frame_0001.png",
         "test_data/pictures/alley_1_frame_0002.png");
  db.Add("test_data/pictures/alley_1_frame_0001.png",
         "test_data/pictures/alley_1_frame_0002.png");

  OPAL opal(db);
}
