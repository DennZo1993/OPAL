#pragma once

#include "Matrix.h"
#include "ImageDatabase.h"
#include <stdexcept>

// Main class in the project. Implements core logic.
class OPAL {
public:
  // Fixed types.
  using ImagePixelType              = double;
  using SegmentationPixelType       = int;
  using DatabaseType = ImageDatabase<ImagePixelType, SegmentationPixelType>;

  // OPAL works with several images stored in a database.
  OPAL(const DatabaseType &database);

private:
  // Database of images. Image to be segmented at index 0.
  const DatabaseType &Database;

  // Output displacement fields.
  Matrix<int>     FieldX; // x-coordinate
  Matrix<int>     FieldY; // y-coordinate
  Matrix<size_t>  FieldT; // image index (in database)
  Matrix<double>  SSDMap; // SSD between patches with centers at (i,j).

  // Size of the images in the database. Just to replace call to
  // Database.getImageHeight(), Database.getImageWidth().
  size_t ImageHeight;
  size_t ImageWidth;
};
