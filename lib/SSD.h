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
  using ValueType = typename DatabaseType::ImgPixelType;

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
  ValueType GetValue() const;

  /**
   * @returns Retult of GetValue().
   */
  operator ValueType() const;

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
  ValueType ShiftRight();

  /**
   * @brief Same moving image, (x-1,y) on both fixed and moving images.
   *
   * @returns Calculated flag.
   */
  ValueType ShiftLeft();

  /**
   * @brief Same moving image, (x,y-1) on both fixed and moving images.
   *
   * @returns Calculated flag.
   */
  ValueType ShiftUp();

  /**
   * @brief Same moving image, (x,y+1) on both fixed and moving images.
   *
   * @returns Calculated flag.
   */
  ValueType ShiftDown();

  /**
   * @brief Same (x,y) coordinates on database[movingIndex - 1].
   *
   * @returns Calculated flag.
   */
  ValueType ShiftTop();

  /**
   * @brief Same (x,y) coordinates on database[movingIndex + 1].
   *
   * @returns Calculated flag.
   */
  ValueType ShiftBottom();

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

  /// Cached SSD value.
  ValueType value;
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
  , patchSide(2 * radius + 1)
  , fixedTopLeftX(ctrFixedX - radius)
  , fixedTopLeftY(ctrFixedY - radius)
  , movingTopLeftX(ctrMovingX - radius)
  , movingTopLeftY(ctrMovingY - radius)
  , value(0)
{
  assert(idx > 0 && "SSD between patches of 0-th image in database!");

  CalculateValue();
}


template <class DatabaseType>
typename SSD<DatabaseType>::ValueType SSD<DatabaseType>::GetValue() const
{
  return value;
}


template <class DatabaseType>
SSD<DatabaseType>::operator ValueType() const
{
  return GetValue();
}


template <class DatabaseType>
bool SSD<DatabaseType>::operator <(const SSD &other) const
{
  return value < other.value;
}


template <class DatabaseType>
typename SSD<DatabaseType>::ValueType SSD<DatabaseType>::ShiftRight()
{
  for (size_t dy = 0; dy < patchSide; ++dy) {
    // Step to new place (1 px right the right side), add it.
    ValueType diff1 =
      (*fixedImageIt)(fixedTopLeftY + dy, fixedTopLeftX + patchSide) -
      (*movingImageIt)(movingTopLeftY + dy, movingTopLeftX + patchSide);

    // Step from old place, subtract it (the left side).
    ValueType diff2 =
      (*fixedImageIt)(fixedTopLeftY + dy, fixedTopLeftX) -
      (*movingImageIt)(movingTopLeftY + dy, movingTopLeftX);

    value += (diff1 * diff1 - diff2 * diff2);
  }
  ++fixedTopLeftX;
  ++movingTopLeftX;

  return value;
}


template <class DatabaseType>
typename SSD<DatabaseType>::ValueType SSD<DatabaseType>::ShiftLeft()
{
  for (size_t dy = 0; dy < patchSide; ++dy) {
    // Step to new place (1 px left the left side), add it.
    ValueType diff1 =
      (*fixedImageIt)(fixedTopLeftY + dy, fixedTopLeftX - 1) -
      (*movingImageIt)(movingTopLeftY + dy, movingTopLeftX - 1);

    // Step from old place, subtract it (the right side).
    ValueType diff2 =
      (*fixedImageIt)(fixedTopLeftY + dy, fixedTopLeftX + patchSide - 1) -
      (*movingImageIt)(movingTopLeftY + dy, movingTopLeftX + patchSide - 1);

    value += (diff1 * diff1 - diff2 * diff2);
  }
  --fixedTopLeftX;
  --movingTopLeftX;

  return value;
}


template <class DatabaseType>
typename SSD<DatabaseType>::ValueType SSD<DatabaseType>::ShiftUp()
{
  for (size_t dx = 0; dx < patchSide; ++dx) {
    // Step to new place (1 px up the upper side), add it.
    ValueType diff1 =
      (*fixedImageIt)(fixedTopLeftY - 1, fixedTopLeftX + dx) -
      (*movingImageIt)(movingTopLeftY - 1, movingTopLeftX + dx);

    // Step from old place (the lower side), subtract it.
    ValueType diff2 =
      (*fixedImageIt)(fixedTopLeftY + patchSide - 1, fixedTopLeftX + dx) -
      (*movingImageIt)(movingTopLeftY + patchSide - 1, movingTopLeftX + dx);

    value += (diff1 * diff1 - diff2 * diff2);
  }
  --fixedTopLeftY;
  --movingTopLeftY;

  return value;
}


template <class DatabaseType>
typename SSD<DatabaseType>::ValueType SSD<DatabaseType>::ShiftDown()
{
  for (size_t dx = 0; dx < patchSide; ++dx) {
    // Step to new place (1 px down the lower side), add it.
    ValueType diff1 =
      (*fixedImageIt)(fixedTopLeftY + patchSide, fixedTopLeftX + dx) -
      (*movingImageIt)(movingTopLeftY + patchSide, movingTopLeftX + dx);

    // Step from old place (upper side), subtract it.
    ValueType diff2 =
      (*fixedImageIt)(fixedTopLeftY, fixedTopLeftX + dx) -
      (*movingImageIt)(movingTopLeftY, movingTopLeftX + dx);

    value += (diff1 * diff1 - diff2 * diff2);
  }
  ++fixedTopLeftY;
  ++movingTopLeftY;

  return value;
}


template <class DatabaseType>
typename SSD<DatabaseType>::ValueType SSD<DatabaseType>::ShiftTop()
{
  --movingImageIt;
  CalculateValue();

  return value;
}


template <class DatabaseType>
typename SSD<DatabaseType>::ValueType SSD<DatabaseType>::ShiftBottom()
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
      ValueType diff = (*fixedImageIt)(fixedTopLeftY + dy, fixedTopLeftX + dx) -
                       (*movingImageIt)(movingTopLeftY + dy, movingTopLeftX + dx);
      value += diff * diff;
    }
}

