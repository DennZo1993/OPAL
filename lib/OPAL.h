#pragma once

#include "ImageDatabase.h"
#include "OPALSettings.h"
#include "SSDObject.h"

#include <chrono>
#include <random>
#include <functional>

// Main class in the project. Implements core logic.
class OPAL {
public:
  // Fixed types.
  using ImagePixelType              = double;
  using SegmentationPixelType       = int;
  using DatabaseType = ImageDatabase<ImagePixelType, SegmentationPixelType>;
  using ImageType = DatabaseType::ImageType;
  using SegmentationType = DatabaseType::SegmentationType;
  using SSDType = SSDObject<ImagePixelType>;

  // OPAL works with several images stored in a database.
  OPAL(const OPALSettings &settings, const DatabaseType &database);

  //  I. Constraint initialization.
  //  For each of the image located at (x, y) a random patch correspondence
  //  located at {(x',y'), t'} where t' is the index of template in the library.
  void ConstrainedInitialization();

  //  II. Propagation step with fast distance computation.
  //  For each patch consider its neighbors to determine if they provide a
  //  better match.
  void Propagation(size_t iteration);


  void BuildSegmentation();


  Image<SegmentationPixelType> GetOutput() const {
    return OutputSegmentation;
  }


  void Run();


  const Image<int>      & getFieldX() const { return FieldX; }
  const Image<int>      & getFieldY() const { return FieldY; }
  const Image<size_t>   & getFieldT() const { return FieldT; }
  const Image<SSDType>  & getSSDMap() const { return SSDMap; }


private:
  // Settings.
  const OPALSettings &Sets;

  // Database of images. Image to be segmented at index 0.
  const DatabaseType &Database;

  // Output displacement fields.
  Image<int>     FieldX; // x-coordinate *offset*
  Image<int>     FieldY; // y-coordinate *offset*
  Image<size_t>  FieldT; // image index (in database)
  Image<SSDType> SSDMap; // SSD between patches with centers at (i,j).

  // Size of the images in the database. Just to replace call to
  // Database.getImageHeight(), Database.getImageWidth().
  size_t ImageHeight;
  size_t ImageWidth;

  // Input image to be segmented. Always Database[0].
  // offsetX <- FieldX[i][j], offsetY <- FieldY[i][j], T <- FieldT[i][j]
  // InputImage[i][j] -> Database.getImage(T)[i+offsetY][j + offsetX]
  Image<ImagePixelType> InputImage;

  Image<SegmentationPixelType> OutputSegmentation;

  using RandomGeneratorType = std::mt19937;
  RandomGeneratorType randGen;

private:
  int PropagatePixel(size_t i, size_t j, int delta);

  // Save current displacement fields.
  void SaveCurrentFields(const std::string &fileName) const;

  // Calculate SSD at InputImage[i][j].
  SSDType SSDAt(size_t i, size_t j) const;

  // Recalculate the whole SSD map.
  void UpdateSSDMap();
};
