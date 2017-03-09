#include "Common.h"
#include "ImageDatabase.h"


TEST(ImageDatabaseTests, TestLoadPNGImage) {
  ImageDatabase<double, int> db;
  db.Add("test_data/pictures/alley_1_frame_0001.png",
         "test_data/pictures/alley_1_frame_0002.png");

  ASSERT_EQ(1, db.GetImageCount());
  const Image<double> &img = db.GetImage(0);
  const Image<int> &seg = db.GetSegmentation(0);

  ASSERT_TRUE(ImageHasSize(img, 436, 1024));
  ASSERT_TRUE(ImageHasSize(seg, 436, 1024));
}


TEST(ImageDatabaseTests, TestLoadDifferentSize1) {
  ImageDatabase<float, int> db;
  ASSERT_THROW(db.Add("test_data/matrix/3x4_ones_float.txt",
                      "test_data/pictures/small_4x5_color.png"),
               std::runtime_error);
}


TEST(ImageDatabaseTests, TestLoadDifferentSize2) {
  ImageDatabase<float, int> db;
  db.Add("test_data/pictures/alley_1_frame_0001.png",
         "test_data/pictures/alley_1_frame_0002.png");
  ASSERT_THROW(db.Add("test_data/pictures/small_4x5_color.png",
                      "test_data/pictures/small_4x5_color.png"),
               std::runtime_error);
}


TEST(ImageDatabaseTests, TestLoadPNGImageAndPixels) {
  ImageDatabase<double, int> db;
  db.Add("test_data/pictures/small_4x5_color.png",
         "test_data/pictures/small_4x5_color.png");

  ASSERT_EQ(1, db.GetImageCount());
  const Image<double> &img = db.GetImage(0);
  const Image<int> &seg = db.GetSegmentation(0);

  ASSERT_TRUE(ImageHasSize(img, 4, 5));
  ASSERT_TRUE(ImageHasSize(seg, 4, 5));

  // Check that image is converted to gray-scale.
  for (int j = 0; j < img.getWidth(); ++j) {
    // First row - black.
    ASSERT_DOUBLE_EQ(0.0, img(0, j));
    ASSERT_EQ(0, seg(0, j));
    // Second row - red (255, 0, 0).
    ASSERT_DOUBLE_EQ(std::lround(255.0 * 0.21), img(1, j));
    ASSERT_EQ(54, seg(1, j));
    // Third row - green (0, 255, 0).
    ASSERT_DOUBLE_EQ(std::lround(255.0 * 0.72), img(2, j));
    ASSERT_EQ(184, seg(2, j));
    // Fourth row - white (255, 255, 255).
    ASSERT_DOUBLE_EQ(255.0, img(3, j));
    ASSERT_EQ(255, seg(3, j));
  }
}


TEST(ImageDatabaseTests, TestReadFromConfig) {
  ImageDatabase<double, int> db;
  db.ReadFromConfig("test_data/IBSR.json");

  ASSERT_EQ(17, db.GetImageCount());
}
