#pragma once

#include "Image.h"
#include "ImageTypes.h"
#include "ImageIO/ImageIO.h"

#include <stdexcept>
#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>


template<class ImagePixelType, class SegmentationPixelType>
class ImageDatabase {
public:
  using ImageType        = Image<ImagePixelType>;
  using SegmentationType = Image<SegmentationPixelType>;

  ImageDatabase() : imageHeight(0), imageWidth(0) {}


  // Throws std::runtime_error.
  void Add(const std::string &imageFileName, const std::string &segFileName) {
    auto imgMat = ImageIO::ReadImage<ImagePixelType>(imageFileName);
    auto segMat = ImageIO::ReadImage<SegmentationPixelType>(segFileName);

    // Don't add empty images.
    if (imgMat.isEmpty() || segMat.isEmpty())
      throw std::runtime_error("Loaded empty image or segmentation!");

    // Don't add images with different dimensions.
    if (imgMat.getHeight() != segMat.getHeight() ||
        imgMat.getWidth() != segMat.getWidth())
      throw std::runtime_error("Image/segmentation size mismatch!");

    // Check size of images.
    if (images.size() == 0) {
      // This must be the first pair of images.
      imageHeight = imgMat.getHeight();
      imageWidth = imgMat.getWidth();
    } else {
      assert(imageHeight != 0 && imageWidth != 0 &&
             "Only one of image dimensions is 0!");

      // Don't add images which dimensions don't match the base.
      if (imgMat.getHeight() != imageHeight || imgMat.getWidth() != imageWidth)
        throw std::runtime_error(
          "Size of new image/segmentation doesn't suit the database!");
    }

    images.push_back(imgMat);
    segmentations.push_back(segMat);
  }


  // Throws std iostream excetions, std::runtime_error.
  void AppendFilesFromList(const std::string &fileName) {
    std::ifstream ifs(fileName);

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

  // Throws std::out_of_range.
  const ImageType &getImage(size_t i) const {
    if (i >= images.size())
     throw std::out_of_range("Index out of range!");
    return images[i];
  }


  // Throws std::out_of_range.
  const SegmentationType &getSegmentation(size_t i) const {
    if(i >= segmentations.size())
      throw std::out_of_range("Index out of range!");
    return segmentations[i];
  }


  size_t getImageCount() const {
    assert(images.size() == segmentations.size() &&
           "Image and segmentation databases must have the same size!");
    return images.size();
  }


  size_t getImageHeight() const { return imageHeight; }
  size_t getImageWidth() const { return imageWidth; }
  inline bool isEmpty() const {
    assert(images.size() == segmentations.size() &&
           "Image and segmentation databases must have the same size!");
    return images.size() == 0;
  }

private:
  std::vector<ImageType> images;
  std::vector<SegmentationType> segmentations;
  size_t imageHeight;
  size_t imageWidth;
};

