#pragma once

#include "Matrix.h"

#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>


template<class ImagePixelType, class SegmentationPixelType>
class ImageDatabase {
public:
  using ImageType = Matrix<ImagePixelType>;
  using SegmentationType = Matrix<SegmentationPixelType>;

  ImageDatabase() {
  }

  void Add(const std::string &imageFileName, const std::string &segFileName) {
    Matrix<ImagePixelType> imgMat;
    imgMat.ReadFromFile(imageFileName);
    images.push_back(imgMat);

    Matrix<SegmentationPixelType> segMat;
    segMat.ReadFromFile(segFileName);
    segmentations.push_back(segMat);

    assert(images.size() == segmentations.size() &&
           "Image and segmentation databases must have the same size!");
  }

  void ReadFilesFromList(const std::string &fileName) {
    std::ifstream ifs(fileName);
    std::string imageFileName, segFileName;
    while (ifs >> imageFileName) {
      if (!ifs >> segFileName)
        break;
      Add(imageFileName, segFileName);
    }
    ifs.close();
  }

  void Clear() {
    images.clear();
    segmentations.clear();
  }

  const ImageType &getImage(int i) const {
    assert(i > 0 && i < images.size() && "Index out of range!");
    return images[i];
  }

  const SegmentationType &getSegmentation(int i) const {
    assert(i > 0 && i < segmentations.size() && "Index out of range!");
    return segmentations[i];
  }

  int getImageCount() const {
    assert(images.size() == segmentations.size() &&
           "Image and segmentation databases must have the same size!");
    return images.size();
  }

private:
  std::vector<ImageType> images;
  std::vector<SegmentationType> segmentations;
};

