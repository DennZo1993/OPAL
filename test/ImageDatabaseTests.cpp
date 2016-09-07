#include "gtest/gtest.h"
#include "ImageDatabase.h"

TEST(ImageDatabaseTests, TestLoadMatrixImage) {
  ImageDatabase<double, int> db;
  db.Add("test_data/matrix/3x4_ones_float.txt", "test_data/matrix/3x4_ones_int.txt");

  ASSERT_EQ(1, db.getImageCount());

  const Matrix<double> &img = db.getImage(0);
  const Matrix<int> &seg = db.getSegmentation(0);
  ASSERT_EQ(3, img.getHeight());
  ASSERT_EQ(4, img.getWidth());
  ASSERT_EQ(3, seg.getHeight());
  ASSERT_EQ(4, seg.getWidth());

  for (int i = 0; i < img.getHeight(); ++i) {
    for (int j = 0; j < img.getWidth(); ++j) {
      ASSERT_DOUBLE_EQ(1.0, img[i][j]);
      ASSERT_EQ(1, seg[i][j]);
    }
  }
}

TEST(ImageDatabaseTests, TestLoadMatrixImagesFromList) {
  ImageDatabase<double, int> db;
  db.ReadFilesFromList("test_data/DatabaseFiles.txt");

  ASSERT_EQ(2, db.getImageCount());

  for (int imageIndex = 0; imageIndex < db.getImageCount(); ++imageIndex) {
    const Matrix<double> &img = db.getImage(imageIndex);
    const Matrix<int> &seg = db.getSegmentation(imageIndex);
    ASSERT_EQ(3, img.getHeight());
    ASSERT_EQ(4, img.getWidth());
    ASSERT_EQ(3, seg.getHeight());
    ASSERT_EQ(4, seg.getWidth());

    for (int i = 0; i < img.getHeight(); ++i) {
      for (int j = 0; j < img.getWidth(); ++j) {
        ASSERT_DOUBLE_EQ(double(imageIndex + 1), img[i][j]);
        ASSERT_EQ(imageIndex + 1, seg[i][j]);
      }
    }
  }
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