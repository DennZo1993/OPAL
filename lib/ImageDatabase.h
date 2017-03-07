#pragma once

#include "Image.h"
#include "ImageIO/ImageIO.h"
#include "util.h" // ReadFileToString

#include <stdexcept>
#include <cassert>
#include <vector>
#include <string>
#include <fstream>


template<class I, class S>
class ImageDatabase {

public:
  using ImagePixelType        = I;
  using SegmentationPixelType = S;
  using ImageType             = Image<ImagePixelType>;
  using SegmentationType      = Image<SegmentationPixelType>;

  ImageDatabase() = default;

public:
  // Add a pair of image and its segmentation. Reads both from files.
  //
  // Throws std::runtime_error.
  void Add(const std::string &imageFileName, const std::string &segFileName);

  // Read a JSON config file and fill the database accordingly.
  void ReadFromConfig(const std::string &fileName);

  // Read a list of filenames as 'image, segmentation' pairs and add them.
  // Doesn't clear the database before execution, adds images to existing ones.
  //
  // Throws std iostream excetions, std::runtime_error.
  void AppendFilesFromList(const std::string &fileName);

  // Clear the database and read from list.
  //
  // Throws std iostream excetions, std::runtime_error.
  void ReadFilesFromList(const std::string &fileName);

  // Clear the contents of the database.
  void Clear();

  // Getters.

  // Get images.
  const ImageType        & GetImage(size_t i)        const;
  const SegmentationType & GetSegmentation(size_t i) const;

  size_t GetImageCount() const;
  inline bool IsEmpty() const;

  // Get image dimensions.
  size_t GetImageHeight() const { return imageHeight; }
  size_t GetImageWidth()  const { return imageWidth;  }

private:
  std::vector<ImageType>        images;
  std::vector<SegmentationType> segmentations;
  size_t imageHeight;
  size_t imageWidth;
};


template<class I, class S>
void ImageDatabase<I, S>::Add(const std::string &imageFileName,
                              const std::string &segFileName)
{
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


template<class I, class S>
void ImageDatabase<I, S>::AppendFilesFromList(const std::string &fileName)
{
  std::ifstream ifs(fileName);

  std::string imageFileName, segFileName;
  while (ifs >> imageFileName) {
    if (!(ifs >> segFileName))
      break;
    Add(imageFileName, segFileName);
  }
  ifs.close();
}


template<class I, class S>
void ImageDatabase<I, S>::ReadFilesFromList(const std::string &fileName)
{
  Clear();
  AppendFilesFromList(fileName);
}


template<class I, class S>
void ImageDatabase<I, S>::Clear()
{
  images.clear();
  segmentations.clear();
  imageHeight = imageWidth = 0;
}

// Throws std::out_of_range.
template<class I, class S>
const typename ImageDatabase<I, S>::ImageType &
ImageDatabase<I, S>::GetImage(size_t i) const
{
  assert(i < images.size() && "Image index is out of range!");

  return images[i];
}


// Throws std::out_of_range.
template<class I, class S>
const typename ImageDatabase<I, S>::SegmentationType &
ImageDatabase<I, S>::GetSegmentation(size_t i) const
{
  assert(i < segmentations.size() && "Segmentation index is out of range!");

  return segmentations[i];
}


template<class I, class S>
size_t ImageDatabase<I, S>::GetImageCount() const
{
  assert(images.size() == segmentations.size() &&
         "Image and segmentation databases must have the same size!");
  return images.size();
}


template<class I, class S>
inline bool ImageDatabase<I, S>::IsEmpty() const
{
  assert(images.size() == segmentations.size() &&
         "Image and segmentation databases must have the same size!");
  return images.size() == 0;
}

