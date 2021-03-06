#include "OPAL.h"
#include "../Common.h"

TEST(OPAL, Initialization) {
  OPALSettings settings = OPALSettings::GetDefaults();
  OPAL::DatabaseType db;
  db.Add("test_data/pictures/alley_1_frame_0001.png",
         "test_data/pictures/alley_1_frame_0001.png");
  db.Add("test_data/pictures/alley_1_frame_0002.png",
         "test_data/pictures/alley_1_frame_0002.png");

  OPAL opal(settings, db);

  opal.ConstrainedInitialization();

  const auto &fieldT = opal.getFieldT();
  ASSERT_TRUE(ImageHasSize(fieldT, 436, 1024));
  ASSERT_TRUE(ImageIsFilledWith(fieldT, 1)); // only 2 pairs in DB.

  const auto &fieldX = opal.getFieldX();
  const auto &fieldY = opal.getFieldY();
  ASSERT_TRUE(ImageHasSize(fieldX, 436, 1024));
  ASSERT_TRUE(ImageHasSize(fieldY, 436, 1024));

  for (size_t i = 0; i < fieldX.getHeight(); ++i) {
    for (size_t j = 0; j < fieldX.getWidth(); ++j) {
      // Check that
      //   1. Both offsets lie within initialization window.
      //   2. Each pixel is mapped so that we can take a patch around it.
      auto offsetX = fieldX(i, j);
      auto offsetY = fieldY(i, j);

      ASSERT_LE(std::abs(offsetX), settings.initWindowRadius);
      ASSERT_LE(std::abs(offsetY), settings.initWindowRadius);

      ASSERT_GE(i + offsetY, settings.patchRadius);
      ASSERT_GE(j + offsetX, settings.patchRadius);

      ASSERT_LT(i + offsetY, fieldX.getHeight());
      ASSERT_LT(j + offsetX, fieldX.getWidth());
    } // for(j)
  } // for(i)

  const auto &ssdMap = opal.getSSDMap();
  ASSERT_TRUE(ImageHasSize(ssdMap, 436, 1024));

  #if 0
  // Check SSDMap
  for (size_t i = settings.patchRadius; i+settings.patchRadius < fieldX.getHeight(); ++i) {
    for (size_t j = settings.patchRadius; j+settings.patchRadius < fieldX.getWidth(); ++j) {
      auto ssd = ssdMap(i, j);
      auto x = j + fieldX(i, j);
      auto y = i + fieldY(i, j);
      ASSERT_DOUBLE_EQ(ssd, db.getImage(0).SSD(db.getImage(1),
                                               i-settings.patchRadius, j-settings.patchRadius,
                                               y-settings.patchRadius, x-settings.patchRadius,
                                               settings.patchSide, settings.patchSide));
    } // for(i)
  } // for(j)
  # endif
      
} // test body
