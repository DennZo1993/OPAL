#pragma once

#include "Matrix.h"
#include "ImageDatabase.h"
#include "OPALSettings.h"

#include <chrono>
#include <random>

// Main class in the project. Implements core logic.
class OPAL {
public:
  // Fixed types.
  using ImagePixelType              = double;
  using SegmentationPixelType       = int;
  using DatabaseType = ImageDatabase<ImagePixelType, SegmentationPixelType>;

  // OPAL works with several images stored in a database.
  OPAL(const OPALSettings &settings, const DatabaseType &database);

  //  I. Constraint initialization.
  //  For each of the image located at (x, y) a random patch correspondence
  //  located at {(x',y'), t'} where t' is the index of template in the library.
  void ConstrainedInitialization();


  const Matrix<int>     &getFieldX() const { return FieldX; }
  const Matrix<int>     &getFieldY() const { return FieldY; }
  const Matrix<size_t>  &getFieldT() const { return FieldT; }
  const Matrix<double>  &getSSDMap() const { return SSDMap; }


private:
  // Settings.
  const OPALSettings &Sets;

  // Database of images. Image to be segmented at index 0.
  const DatabaseType &Database;

  // Output displacement fields.
  Matrix<int>     FieldX; // x-coordinate *offset*
  Matrix<int>     FieldY; // y-coordinate *offset*
  Matrix<size_t>  FieldT; // image index (in database)
  Matrix<double>  SSDMap; // SSD between patches with centers at (i,j).

  // Size of the images in the database. Just to replace call to
  // Database.getImageHeight(), Database.getImageWidth().
  size_t ImageHeight;
  size_t ImageWidth;

  // Input image to be segmented. Always Database[0].
  // offsetX <- FieldX[i][j], offsetY <- FieldY[i][j], T <- FieldT[i][j]
  // InputImage[i][j] -> Database.getImage(T)[i+offsetY][j + offsetX]
  Matrix<ImagePixelType> InputImage;

  using RandomGeneratorType = std::mt19937;
  RandomGeneratorType randGen;

private:
  // Save current displacement fields.
  void SaveCurrentFields(const std::string &fileName) const;

  // Recalculate the whole SSD map.
  void UpdateSSDMap();
};
