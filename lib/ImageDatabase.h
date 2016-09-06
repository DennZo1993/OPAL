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

  ImageDatabase() {}

  void Add(const std::string &imageFileName, const std::string &segFileName) {
    std::ifstream ifsImg(imageFileName);
    std::ifstream ifsSeg(segFileName);
    if (ifsImg.fail() || ifsSeg.fail())
      return;

    Matrix<ImagePixelType> imgMat;
    Matrix<SegmentationPixelType> segMat;

    ifsImg >> imgMat;
    ifsSeg >> segMat;
    images.push_back(imgMat);
    segmentations.push_back(segMat);

    assert(images.size() == segmentations.size() &&
           "Image and segmentation databases must have the same size!");

    ifsImg.close();
    ifsSeg.close();
  }

  void AppendFilesFromList(const std::string &fileName) {
    std::ifstream ifs(fileName);
    if (ifs.fail())
      return;

    std::string imageFileName, segFileName;
    while (ifs >> imageFileName) {
      if (!(ifs >> segFileName))
        break;
      Add(imageFileName, segFileName);
    }
    ifs.close();
  }

  void ReadFilesFromList(const std::string &fileName) {
    Clear();
    AppendFilesFromList(fileName);
  }

  void Clear() {
    images.clear();
    segmentations.clear();
  }

  const ImageType &getImage(int i) const {
    assert(i >= 0 && i < images.size() && "Index out of range!");
    return images[i];
  }

  const SegmentationType &getSegmentation(int i) const {
    assert(i >= 0 && i < segmentations.size() && "Index out of range!");
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

