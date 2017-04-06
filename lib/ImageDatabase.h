#pragma once

#include "Image.h"
#include "ImageIO/ImageIO.h"
#include "util.h" // ReadFileToString
#include "json11/json11.hpp"

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

  using ImageType        = Image<ImagePixelType>;
  using SegmentationType = Image<SegmentationPixelType>;

  template<class T>
  struct FileImage {
    std::string name;
    Image<T> image;
  };

  using FileImageType        = FileImage<ImagePixelType>;
  using FileSegmentationType = FileImage<SegmentationPixelType>;


  ImageDatabase() = default;

public:
  /**
   * @brief Adds a pair of image and its segmentation. Reads both from files.
   *
   * @param [in] imageFileName Name of image file.
   * @param [in] segFileName Name of segmentation file.
   *
   * @throws std::runtime_error.
   */
  void Add(const std::string &imageFileName, const std::string &segFileName);

  /**
   * @brief Adds a pair "image, its segmentation" without reading files from
   * file system.
   *
   * Names of files can be passed, but are used only for logging.
   * The following conditions must be met for successful run:
   *   1. Image and segmentation must be non-empty.
   *   2. Size of image matches size of segmentation.
   *   3. Size of image matches size of other images in the database.
   *
   * @param [in] imgMat Image.
   * @param [in] segMat Segmentation.
   * @param [in] imageFileName Name of image file (optional).
   * @param [in] segFileName Name of segmentation file (optional).
   *
   * @throws std::runtime_error.
   */
  void Add(const ImageType &imgMat,
           const SegmentationType &segMat,
           const std::string &imageFileName = "",
           const std::string &segFileName = "");

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
  inline const ImageType        & GetImage(size_t i)        const;
  inline const SegmentationType & GetSegmentation(size_t i) const;

  inline size_t GetImageCount() const;
  inline bool   IsEmpty()       const;

  // Get image dimensions.
  size_t GetImageHeight() const { return imageHeight; }
  size_t GetImageWidth()  const { return imageWidth;  }

private:
  // Parse 'images' or 'segmentation' section. Section is required.
  // files and folder are cleared first.
  //
  // Throws std::runtime_error if section is not present in the config.
  void ParseConfigSection(const json11::Json &config,
                          const std::string &section,
                          std::vector<std::string> &absFiles);

private:
  std::vector<FileImageType>        images;
  std::vector<FileSegmentationType> segmentations;

  size_t imageHeight;
  size_t imageWidth;

  std::string dbName;
};


template<class I, class S>
void ImageDatabase<I, S>::Add(const std::string &imageFileName,
                              const std::string &segFileName)
{
  auto imgMat = ImageIO::ReadImage<ImagePixelType>(imageFileName);
  auto segMat = ImageIO::ReadImage<SegmentationPixelType>(segFileName);

  Add(imgMat, segMat, imageFileName, segFileName);
}


template <class I, class S>
void ImageDatabase<I, S>::Add(const ImageType &imgMat,
                              const SegmentationType &segMat,
                              const std::string &imageFileName,
                              const std::string &segFileName)
{
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

  images.push_back({imageFileName, imgMat});
  segmentations.push_back({segFileName, segMat});
}


template<class I, class S>
void ImageDatabase<I, S>::ReadFromConfig(const std::string &fileName)
{
  std::string config = util::ReadFileToString(fileName);
  std::string parseError;
  auto jsonConfig = json11::Json::parse(config, parseError);

  if (jsonConfig.is_null())
    throw std::runtime_error("Error while parsing JSON from " + fileName +
                             " - " + parseError);

  // Retrieve info from config.

  // Database name (optional).
  auto name = jsonConfig["name"];
  if (!name.is_null())
    dbName = name.string_value();

  std::vector<std::string> imagesFiles;
  ParseConfigSection(jsonConfig, "images", imagesFiles);

  std::vector<std::string> segFiles;
  ParseConfigSection(jsonConfig, "segmentations", segFiles);

  if (imagesFiles.size() != segFiles.size())
    throw std::runtime_error("Images / segmentations files count mismatch");

  // Read files.
  for (size_t i = 0; i < imagesFiles.size(); ++i)
    Add(imagesFiles[i], segFiles[i]);
}


template<class I, class S>
void ImageDatabase<I, S>::ParseConfigSection(const json11::Json &config,
                                             const std::string &section,
                                             std::vector<std::string> &absFiles)
{
  std::string folder = "";
  std::string extension = "";
  std::vector<std::string> configFiles;
  absFiles.clear();

  // Section is required.
  auto jsonSection = config[section];
  if (jsonSection.is_null())
    throw std::runtime_error("'" + section + "' section not found in config");

  // Example:
  // "images": {
  //   "files": [
  //     "1.img",
  //     "2.img"
  //   ],
  //   "folder": "/home/d-zobnin/images"
  // }

  // At least one of 'files' and 'folder' subsections must be given.
  auto folderSection = jsonSection["folder"];
  auto filesSection = jsonSection["files"];
  auto extensionSection = jsonSection["extension"];
  if (!folderSection.is_string() && !filesSection.is_array())
    throw std::runtime_error("'files' and 'folder' subsections not found for '"
                             + section + "' section");

  if (folderSection.is_string())
    folder = folderSection.string_value();

  if (extensionSection.is_string())
    extension = extensionSection.string_value();

  if (filesSection.is_array())
    for (const auto &item : filesSection.array_items()) {
      if (!item.is_string())
        throw std::runtime_error("'files' section must contain only strings");
      configFiles.push_back(item.string_value());
    }

  // If 'files' is not given, read all files from 'folder'.
  if (configFiles.empty()) {
    absFiles = util::ListDir(folder, extension, /*sorted=*/ true);
  } else {
    for (const auto &f : configFiles)
      absFiles.push_back(util::ConcatPaths(folder, f));
  }
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

  return images[i].image;
}


// Throws std::out_of_range.
template<class I, class S>
const typename ImageDatabase<I, S>::SegmentationType &
ImageDatabase<I, S>::GetSegmentation(size_t i) const
{
  assert(i < segmentations.size() && "Segmentation index is out of range!");

  return segmentations[i].image;
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

