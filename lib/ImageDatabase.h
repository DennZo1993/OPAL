#pragma once

#include "Matrix.h"
#include "lodepng/lodepng.h"

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
    Matrix<ImagePixelType> imgMat;
    Matrix<SegmentationPixelType> segMat;

    if (filenameHasExtension(imageFileName, ".png")) {
      imgMat = ReadMatrixFromPNG<ImagePixelType>(imageFileName);
    } else {
      std::ifstream ifsImg(imageFileName);
      if (ifsImg.fail())
        return;
      ifsImg >> imgMat;
      ifsImg.close();
    }

    if (filenameHasExtension(segFileName, ".png")) {
      segMat = ReadMatrixFromPNG<SegmentationPixelType>(segFileName);
    } else {
      std::ifstream ifsSeg(segFileName);
      if (ifsSeg.fail())
        return;
      ifsSeg >> segMat;
      ifsSeg.close();
    }

    assert(imgMat.getHeight() == segMat.getHeight() &&
           imgMat.getWidth() == segMat.getWidth() &&
           "Image and segmentation sizes must be the same!");

    images.push_back(imgMat);
    segmentations.push_back(segMat);

    assert(images.size() == segmentations.size() &&
           "Image and segmentation databases must have the same size!");
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
  // Reads PNG image from file /p fileName. Returns empty Matrix on error.
  // Converts RGB image to grayscale.
  template<class PixelType>
  Matrix<PixelType> ReadMatrixFromPNG(const std::string &fileName) const {
    std::vector<unsigned char> png;
    std::vector<unsigned char> image;
    unsigned width = 0, height = 0;
    // Return empty matrix on error.
    if (lodepng::load_file(png, fileName))
      return Matrix<PixelType>();
    // Return empty matrix on error.
    if (lodepng::decode(image, width, height, png))
      return Matrix<PixelType>();

    // Construct new Matrix. Convert RGB to grayscale.
    Matrix<PixelType> result(height, width, 0);
    for (unsigned y = 0; y < height; ++y) {
      for (unsigned x = 0; x < width; ++x) {
        auto R = image[4 * width * y + 4 * x + 0];
        auto G = image[4 * width * y + 4 * x + 1];
        auto B = image[4 * width * y + 4 * x + 2];
        auto gray = (R * 0.3) + (G * 0.59) + (B * 0.11);
        result[y][x] = gray;
      }
    }

    return result;
  }

  // Returns true if filename /p source has extension /p ext.
  // Precondition: ext must start with '.'
  bool filenameHasExtension(const std::string &source, const std::string &ext) const {
    assert(ext.length() > 0 && "Extension is empty!");
    assert(ext[0] == '.' && "Extension must start with '.' !");
    // source must represent a valid (non-empty) file name. So at least 1 character + ext.
    if (source.length() < ext.length() + 1)
      return false;
    return source.substr(source.length() - ext.length(), ext.length()) == ext;
  }

  std::vector<ImageType> images;
  std::vector<SegmentationType> segmentations;
};

