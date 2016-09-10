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

  ImageDatabase() : imageHeight(0), imageWidth(0) {}

  void Add(const std::string &imageFileName, const std::string &segFileName) {
    Matrix<ImagePixelType> imgMat;
    Matrix<SegmentationPixelType> segMat;
    imgMat.ReadFromFile(imageFileName);
    segMat.ReadFromFile(segFileName);

    // Don't add empty images.
    // FIXME: Throw an exception here!
    if (imgMat.isEmpty() || segMat.isEmpty())
      return;
    // Don't add images with different dimensions.
    // FIXME: Throw an exception here!
    if (imgMat.getHeight() != segMat.getHeight() ||
        imgMat.getWidth() != segMat.getWidth())
      return;

    // Check size of images.
    if (images.size() == 0) {
      // This must be the first pair of images.
      imageHeight = imgMat.getHeight();
      imageWidth = imgMat.getWidth();
    } else {
      assert(imageHeight != 0 && imageWidth != 0 &&
             "Only one of image dimensions is 0!");
      // Don't add images which dimensions don't match the base.
      // FIXME: Throw an exception here!
      if (imgMat.getHeight() != imageHeight || imgMat.getWidth() != imageWidth)
        return;
    }

    // Here we must have 2 non-empty images with the same dimensions that fit the
    // dimensions of database.
    assert(!imgMat.isEmpty() && !segMat.isEmpty() && 
           "At least one of the images is empty!");
    assert(imgMat.getHeight() == segMat.getHeight() &&
           imgMat.getWidth() == segMat.getWidth() &&
           "Image and segmentation sizes must be the same!");
    assert(imgMat.getHeight() == imageHeight && imgMat.getWidth() == imageWidth &&
           "Image size doesn't match the size of other images in database!");

    images.push_back(imgMat);
    segmentations.push_back(segMat);

    assert(images.size() == segmentations.size() &&
           "Image and segmentation databases must have the same size!");
  }

  void AppendFilesFromList(const std::string &fileName) {
    std::ifstream ifs(fileName);
    // FIXME: exception here!
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
    imageHeight = imageWidth = 0;
  }

  const ImageType &getImage(unsigned i) const {
    assert(i < images.size() && "Index out of range!");
    return images[i];
  }

  const SegmentationType &getSegmentation(unsigned i) const {
    assert(i < segmentations.size() && "Index out of range!");
    return segmentations[i];
  }

  unsigned getImageCount() const {
    assert(images.size() == segmentations.size() &&
           "Image and segmentation databases must have the same size!");
    return images.size();
  }

  unsigned getImageHeight() const { return imageHeight; }
  unsigned getImageWidth() const { return imageWidth; }

private:
  std::vector<ImageType> images;
  std::vector<SegmentationType> segmentations;
  unsigned imageHeight;
  unsigned imageWidth;
};

