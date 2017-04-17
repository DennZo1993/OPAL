/**
 * @file lib/SSD.h
 *
 * @author Denis Zobnin
 *
 * @brief Header file with definition of SSD class.
 */


#pragma once

#include <cassert>
#include <cstddef>

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
  using PixelType = typename DatabaseType::ImgPixelType;

public:
  // Constructors and destructors.

  SSD() = default;

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
   * @todo Documentation.
   */
  bool operator <(const SSD &other) const;

public:
  // Methods for efficient update.

  /**
   * @brief Same moving image, (x+1,y) on both fixed and moving images.
   *
   * @returns Calculated flag.
   */
  PixelType ShiftRight();

  /**
   * @brief Same moving image, (x-1,y) on both fixed and moving images.
   *
   * @returns Calculated flag.
   */
  PixelType ShiftLeft();

  /**
   * @brief Same moving image, (x,y-1) on both fixed and moving images.
   *
   * @returns Calculated flag.
   */
  PixelType ShiftUp();

  /**
   * @brief Same moving image, (x,y+1) on both fixed and moving images.
   *
   * @returns Calculated flag.
   */
  PixelType ShiftDown();

  /**
   * @brief Same (x,y) coordinates on database[movingIndex - 1].
   *
   * @returns Calculated flag.
   */
  PixelType ShiftTop();

  /**
   * @brief Same (x,y) coordinates on database[movingIndex + 1].
   *
   * @returns Calculated flag.
   */
  PixelType ShiftBottom();

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
  bool PatchInsideImage(const size_t x, const size_t y);

private:
  /// Fixed image iterator. Doesn't move, always img_cbegin().
  typename DatabaseType::ConstImgIterator fixedImageIt;

  /// Moving image iterator (img_cbegin() + movingIndex).
  typename DatabaseType::ConstImgIterator movingImageIt;

  /// Iterator pointing at the end of database.
  typename DatabaseType::ConstImgIterator databaseEnd;

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
};


// ===== Implementation below =====

template <class DatabaseType>
SSD<DatabaseType>::SSD(const DatabaseType &db, size_t idx,
                       size_t ctrFixedX, size_t ctrFixedY,
                       size_t ctrMovingX, size_t ctrMovingY,
                       size_t radius)
  : fixedImageIt(db.img_cbegin())
  , movingImageIt(db.img_cbegin() + idx)
  , databaseEnd(db.img_cend())
  , patchRadius(radius)
  , patchSide(2 * patchRadius + 1)
  , fixedTopLeftX(ctrFixedX - patchRadius)
  , fixedTopLeftY(ctrFixedY - patchRadius)
  , movingTopLeftX(ctrMovingX - patchRadius)
  , movingTopLeftY(ctrMovingY - patchRadius)
  , value(0)
{
  assert(idx > 0 && "SSD between patches of 0-th image in database!");

  imageHeight = db.GetImageHeight();
  imageWidth  = db.GetImageWidth();

  // If we try to construct a new SSD, be sure we pass the correct arguments to
  // calculate it.
  assert(PatchInsideImage(fixedTopLeftX, fixedTopLeftY) &&
         "Patch is outside fixed image!");
  assert(PatchInsideImage(movingTopLeftX, movingTopLeftY) &&
         "Patch is outside moving image!");

  CalculateValue();
}


template <class DatabaseType>
typename SSD<DatabaseType>::PixelType SSD<DatabaseType>::GetValue() const
{
  return value;
}


template <class DatabaseType>
SSD<DatabaseType>::operator PixelType() const
{
  return GetValue();
}


template <class DatabaseType>
bool SSD<DatabaseType>::operator <(const SSD &other) const
{
  return value < other.value;
}


template <class DatabaseType>
typename SSD<DatabaseType>::PixelType SSD<DatabaseType>::ShiftRight()
{
  for (size_t dy = 0; dy < patchSide; ++dy) {
    // Step to new place (1 px right the right side), add it.
    PixelType diff1 =
      (*fixedImageIt)(fixedTopLeftY + dy, fixedTopLeftX + patchSide) -
      (*movingImageIt)(movingTopLeftY + dy, movingTopLeftX + patchSide);

    // Step from old place, subtract it (the left side).
    PixelType diff2 =
      (*fixedImageIt)(fixedTopLeftY + dy, fixedTopLeftX) -
      (*movingImageIt)(movingTopLeftY + dy, movingTopLeftX);

    value += (diff1 * diff1 - diff2 * diff2);
  }
  ++fixedTopLeftX;
  ++movingTopLeftX;

  return value;
}


template <class DatabaseType>
typename SSD<DatabaseType>::PixelType SSD<DatabaseType>::ShiftLeft()
{
  for (size_t dy = 0; dy < patchSide; ++dy) {
    // Step to new place (1 px left the left side), add it.
    PixelType diff1 =
      (*fixedImageIt)(fixedTopLeftY + dy, fixedTopLeftX - 1) -
      (*movingImageIt)(movingTopLeftY + dy, movingTopLeftX - 1);

    // Step from old place, subtract it (the right side).
    PixelType diff2 =
      (*fixedImageIt)(fixedTopLeftY + dy, fixedTopLeftX + patchSide - 1) -
      (*movingImageIt)(movingTopLeftY + dy, movingTopLeftX + patchSide - 1);

    value += (diff1 * diff1 - diff2 * diff2);
  }
  --fixedTopLeftX;
  --movingTopLeftX;

  return value;
}


template <class DatabaseType>
typename SSD<DatabaseType>::PixelType SSD<DatabaseType>::ShiftUp()
{
  for (size_t dx = 0; dx < patchSide; ++dx) {
    // Step to new place (1 px up the upper side), add it.
    PixelType diff1 =
      (*fixedImageIt)(fixedTopLeftY - 1, fixedTopLeftX + dx) -
      (*movingImageIt)(movingTopLeftY - 1, movingTopLeftX + dx);

    // Step from old place (the lower side), subtract it.
    PixelType diff2 =
      (*fixedImageIt)(fixedTopLeftY + patchSide - 1, fixedTopLeftX + dx) -
      (*movingImageIt)(movingTopLeftY + patchSide - 1, movingTopLeftX + dx);

    value += (diff1 * diff1 - diff2 * diff2);
  }
  --fixedTopLeftY;
  --movingTopLeftY;

  return value;
}


template <class DatabaseType>
typename SSD<DatabaseType>::PixelType SSD<DatabaseType>::ShiftDown()
{
  for (size_t dx = 0; dx < patchSide; ++dx) {
    // Step to new place (1 px down the lower side), add it.
    PixelType diff1 =
      (*fixedImageIt)(fixedTopLeftY + patchSide, fixedTopLeftX + dx) -
      (*movingImageIt)(movingTopLeftY + patchSide, movingTopLeftX + dx);

    // Step from old place (upper side), subtract it.
    PixelType diff2 =
      (*fixedImageIt)(fixedTopLeftY, fixedTopLeftX + dx) -
      (*movingImageIt)(movingTopLeftY, movingTopLeftX + dx);

    value += (diff1 * diff1 - diff2 * diff2);
  }
  ++fixedTopLeftY;
  ++movingTopLeftY;

  return value;
}


template <class DatabaseType>
typename SSD<DatabaseType>::PixelType SSD<DatabaseType>::ShiftTop()
{
  --movingImageIt;
  CalculateValue();

  return value;
}


template <class DatabaseType>
typename SSD<DatabaseType>::PixelType SSD<DatabaseType>::ShiftBottom()
{
  ++movingImageIt;
  CalculateValue();

  return value;
}


template <class DatabaseType>
void SSD<DatabaseType>::CalculateValue()
{
  value = 0;
  for (size_t dy = 0; dy < patchSide; ++dy)
    for (size_t dx = 0; dx < patchSide; ++dx) {
      PixelType diff = (*fixedImageIt)(fixedTopLeftY + dy, fixedTopLeftX + dx) -
                       (*movingImageIt)(movingTopLeftY + dy, movingTopLeftX + dx);
      value += diff * diff;
    }
}


template <class DatabaseType>
bool SSD<DatabaseType>::PatchInsideImage(const size_t x, const size_t y)
{
  // x and y are coordinates of top-left corner of a patch.
  // Because of size_t type we don't need to check that x >= 0 and y >= 0.
  return (x + patchSide <= imageWidth) && (y + patchSide <= imageHeight);
}
