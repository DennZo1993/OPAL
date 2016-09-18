#include "Common.h"
#include "ImageDatabase.h"

TEST(ImageDatabaseTests, TestLoadMatrixImage) {
  ImageDatabase<double, int> db;
  db.Add("test_data/matrix/3x4_ones_float.txt",
         "test_data/matrix/3x4_ones_int.txt");

  ASSERT_EQ(1, db.getImageCount());

  const Matrix<double> &img = db.getImage(0);
  const Matrix<int> &seg = db.getSegmentation(0);

  ASSERT_TRUE(MatrixHasSize(img, 3, 4));
  ASSERT_TRUE(MatrixHasSize(seg, 3, 4));

  ASSERT_TRUE(MatrixIsFilledWith(img, 1.0));
  ASSERT_TRUE(MatrixIsFilledWith(seg, 1));
}


TEST(ImageDatabaseTests, TestLoadPNGImage) {
  ImageDatabase<double, int> db;
  db.Add("test_data/pictures/alley_1_frame_0001.png",
         "test_data/pictures/alley_1_frame_0002.png");

  ASSERT_EQ(1, db.getImageCount());
  const Matrix<double> &img = db.getImage(0);
  const Matrix<int> &seg = db.getSegmentation(0);

  ASSERT_TRUE(MatrixHasSize(img, 436, 1024));
  ASSERT_TRUE(MatrixHasSize(seg, 436, 1024));
}


TEST(ImageDatabaseTests, TestLoadDifferentSize1) {
  ImageDatabase<float, int> db;
  ASSERT_THROW(db.Add("test_data/matrix/3x4_ones_float.txt",
                      "test_data/pictures/alley_1_frame_0002.png"),
               std::runtime_error);
}


TEST(ImageDatabaseTests, TestLoadDifferentSize2) {
  ImageDatabase<float, int> db;
  db.Add("test_data/pictures/alley_1_frame_0001.png",
         "test_data/pictures/alley_1_frame_0002.png");
  ASSERT_THROW(db.Add("test_data/matrix/3x4_ones_float.txt",
                      "test_data/matrix/3x4_ones_int.txt"),
               std::runtime_error);
}


TEST(ImageDatabaseTests, TestLoadPNGImageAndPixels) {
  ImageDatabase<double, int> db;
  db.Add("test_data/pictures/small_4x5_color.png",
         "test_data/pictures/small_4x5_color.png");

  ASSERT_EQ(1, db.getImageCount());
  const Matrix<double> &img = db.getImage(0);
  const Matrix<int> &seg = db.getSegmentation(0);

  ASSERT_TRUE(MatrixHasSize(img, 4, 5));
  ASSERT_TRUE(MatrixHasSize(seg, 4, 5));

  // Check that image is converted to gray-scale.
  for (int j = 0; j < img.getWidth(); ++j) {
    // First row - black.
    ASSERT_DOUBLE_EQ(0.0, img[0][j]);
    ASSERT_EQ(0, seg[0][j]);
    // Second row - red (255, 0, 0).
    ASSERT_DOUBLE_EQ(255.0 * 0.3, img[1][j]);
    ASSERT_EQ(76, seg[1][j]);
    // Third row - green (0, 255, 0).
    ASSERT_DOUBLE_EQ(255.0 * 0.59, img[2][j]);
    ASSERT_EQ(150, seg[2][j]);
    // Fourth row - white (255, 255, 255).
    ASSERT_DOUBLE_EQ(255.0, img[3][j]);
    ASSERT_EQ(255, seg[3][j]);
  }
}


TEST(ImageDatabaseTests, TestLoadMatrixImagesFromList) {
  ImageDatabase<double, int> db;
  db.ReadFilesFromList("test_data/DatabaseFiles.txt");

  ASSERT_EQ(2, db.getImageCount());

  for (int imageIndex = 0; imageIndex < db.getImageCount(); ++imageIndex) {
    const Matrix<double> &img = db.getImage(imageIndex);
    const Matrix<int> &seg = db.getSegmentation(imageIndex);
    ASSERT_TRUE(MatrixHasSize(img, 3, 4));
    ASSERT_TRUE(MatrixHasSize(seg, 3, 4));

    ASSERT_TRUE(MatrixIsFilledWith(img, imageIndex + 1));
    ASSERT_TRUE(MatrixIsFilledWith(seg, imageIndex + 1));
  }
}


TEST(ImageDatabaseTests, TestLoadMixedImagesFromList) {
  ImageDatabase<double, int> db;
  db.ReadFilesFromList("test_data/MixedDatabaseFiles.txt");

  ASSERT_EQ(1, db.getImageCount());
  const Matrix<double> &img = db.getImage(0);
  const Matrix<int> &seg = db.getSegmentation(0);

  ASSERT_TRUE(MatrixHasSize(img, 4, 5));
  ASSERT_TRUE(MatrixHasSize(seg, 4, 5));
}


TEST(ImageDatabaseTests, TestAppendMatrixImagesFromList) {
  ImageDatabase<double, int> db;
  // First read.
  db.ReadFilesFromList("test_data/DatabaseFiles.txt");
  ASSERT_EQ(2, db.getImageCount());
  // Read destroys previously added images.
  db.ReadFilesFromList("test_data/DatabaseFiles.txt");
  ASSERT_EQ(2, db.getImageCount());
  // Append does not.
  db.AppendFilesFromList("test_data/DatabaseFiles.txt");
  ASSERT_EQ(4, db.getImageCount());
}


TEST(ImageDatabaseTests, TestAccessFail) {
  ImageDatabase<double, int> db;
  db.ReadFilesFromList("test_data/MixedDatabaseFiles.txt");

  ASSERT_THROW(db.getImage(1), std::out_of_range);
  ASSERT_THROW(db.getSegmentation(2), std::out_of_range);
}
