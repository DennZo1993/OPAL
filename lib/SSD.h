/**
 * @file lib/SSD.h
 *
 * @author Denis Zobnin
 *
 * @brief Header file with definition of SSD class.
 */


#pragma once

#include <cassert>


/**
 * @brief Class responsible for SSD calculation between patches of images.
 *
 * Needs a reference to ImageDatabase to construct, as it must support
 * calculating SSD between adjacent pathces in both X and Y dimensions, as well
 * as between patches of adjacent images in database.
 *
 * SSD is constructed from coordinates of centers of patches, but uses
 * coordinates of top-left corner of patches for calculation.
 *
 * Needs only index of a single image in database (>= 1) because the other image
 * is always Database[0] (image to be segmented).
 *
 * @tparam DatabaseType Type of image database. Must be ImageDatabase.
 */
template <class DatabaseType>
class SSD {
public:
  using ImageType = typename DatabaseType::ImageType;
  using PixelType = typename DatabaseType::ImagePixelType;

public:
  // Constructors and destructors.

  /**
   * @brief Constructor from coordinates of centers of patches.
   *
   * Calculates coordinates of top-left corners of patches on images.
   * Patches must lay inside the images, i. e. @p ctrFixedX >= @p radius,
   * ctrFixedX < imageWidth - @p radius, similarly for Y coordinates.
   *
   * @param [in] db Database of images
   * @param [in] idx Index of moving image in database, must be >= 1
   * @param [in] ctrFixedX X-coordinate of center of patch on fixed image
   * @param [in] ctrFixedY Y-coordinate of center of patch on fixed image
   * @param [in] ctrMovingX X-coordinate of center of patch on moving image
   * @param [in] ctrMovingY Y-coordinate of center of patch on moving image
   * @param [in] radius Radius of patches
   */
  SSD(const DatabaseType &db, size_t idx,
      size_t ctrFixedX, size_t ctrFixedY, size_t ctrMovingX, size_t ctrMovingY,
      size_t radius);

  ~SSD() = default;

public:
  // General-purpose methods.

  /**
   * @returns Value of calculated SSD.
   */
  PixelType GetValue() const;

  /**
   * @returns Retult of GetValue().
   */
  operator PixelType() const;

  /**
   * @returns Calculated flag.
   */
  operator bool() const;

  /**
   * @todo Documentation.
   */
  bool operator <(const SSD &other) const;

public:
  // Methods for efficient update.

  bool ShiftRight();

  bool ShiftLeft();

  bool ShiftUp();

  bool ShiftDown();

  bool ShiftTop();

  bool ShiftBottom();

private:

  /**
   * @todo Documentation.
   */
  void CalculateValue();

  /**
   * @returns True if patch is located completely inside an image.
   *
   * @param x X-coordinate of top-left corner of patch
   * @param y Y-coordinate of top-left corner of patch
   */
  static bool PatchInsideImage(const size_t x, const size_t y);

private:
  /// Database of images.
  const DatabaseType &database;

  /// Index of image in database. Must be >= 1.
  size_t movingIndex;

  /// Pointer to fixed image (database[0]).
  const ImageType *fixedImage;

  /// Pointer to moving image (database[movingIndex]).
  const ImageType *movingImage;

  /// Radius of patch (parameter).
  size_t patchRadius;

  /// Side of patch. patchSize = 2 * patchRadius + 1.
  size_t patchSide;

  /// X-coordinate of top-left corner of patch on fixed image (database[0]).
  size_t fixedTopLeftX;

  /// Y-coordinate of top-left corner of patch on fixed image (database[0]).
  size_t fixedTopLeftY;

  /// X-coordinate of top-left corner of patch on moving image.
  size_t movingTopLeftX;

  /// Y-coordinate of top-left corner of patch on moving image.
  size_t movingTopLeftY;

  /// Cached height of images.
  size_t imageHeight;

  /// Cached width of images.
  size_t imageWidth;

  /// Cached SSD value.
  PixelType value;

  /// Cached flag indicating whether this SSD is already calculated or not.
  bool calculated;
};


// ===== Implementation below =====

SSD::SSD(const DatabaseType &db, size_t idx,
         size_t ctrFixedX, size_t ctrFixedY,
         size_t ctrMovingX, size_t ctrMovingY,
         size_t radius)
  : database(db)
  , movingIndex(idx)
  , patchRadius(radius)
  , patchSide(2 * patchRadius + 1)
  , fixedTopLeftX(ctrFixedX - patchRadius)
  , fixedTopLeftY(ctrFixedY - patchRadius)
  , movingTopLeftX(ctrMovingX - patchRadius)
  , movingTopLeftY(ctrMovingY - patchRadius)
  , value(0)
  , calculated(false)
{
  assert(movingIndex > 0 && "SSD between patches of 0-th image in database!");

  fixedImage  = &database.GetImage(0);
  movingImage = &database.GetImage(movingIndex);
  imageHeight = database.GetImageHeight();
  imageWidth  = database.GetImageWidth();

  // If we try to construct a new SSD, be sure we pass the correct arguments to
  // calculate it.
  assert(PatchInsideImage(fixedTopLeftX, fixedTopLeftY) &&
         "Patch is outside fixed image!");
  assert(PatchInsideImage(movingTopLeftX, movingTopLeftY) &&
         "Patch is outside moving image!");

  CalculateValue();

  // After calculation we must have the 'calculated' flag set to true.
  assert(calculated && "SSD is not calculated after construction!");
}


SSD::PixelType SSD::GetValue() const
{
  assert(calculated && "Value query from an invalid SSD object!");

  return value;
}


SSD::operator PixelType() const
{
  return GetValue();
}


SSD::operator bool() const
{
  return calculated;
}


bool SSD::operator <(const SSD &other)
{
  if (!calculated)
    return false;
  if (!other.calculated)
    return true;
  return value < other.value;
}


bool SSD::ShiftRight()
{
  return calculated;
}


bool SSD::ShiftLeft()
{
  return calculated;
}


bool SSD::ShiftUp()
{
  return calculated;
}


bool SSD::ShiftDown()
{
  return calculated;
}


bool SSD::ShiftTop()
{
  return calculated;
}


bool SSD::ShiftBottom()
{
  return calculated;
}


void SSD::CalculateValue()
{
  value = 0;
  for (size_t dy = 0; dy < patchSide; ++dy)
    for (size_t dx = 0; dx < patchSide; ++dx) {
      PixelType diff = (*fixedImage)(fixedTopLeftY + dy, fixedTopLeftX + dx) -
                       (*movingImage)(movingTopLeftY + dy, movingTopLeftX + dx);
      value += diff * diff;
    }
  calculated = true;
}


bool SSD::PatchInsideImage(const size_t x, const size_t y)
{
  // x and y are coordinates of top-left corner of a patch.
  // Because of size_t type we don't need to check that x >= 0 and y >= 0.
  return (x + patchSide <= imageWidth) && (y + patchSide <= imageHeight);
}
