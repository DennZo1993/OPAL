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
  using ImageType = typename DatabaseType::ImgType;
  using PixelType = typename DatabaseType::ImgPixelType;

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

  /**
   * @brief Same moving image, (x+1,y) on both fixed and moving images.
   *
   * @returns Calculated flag.
   */
  bool ShiftRight();

  /**
   * @brief Same moving image, (x-1,y) on both fixed and moving images.
   *
   * @returns Calculated flag.
   */
  bool ShiftLeft();

  /**
   * @brief Same moving image, (x,y-1) on both fixed and moving images.
   *
   * @returns Calculated flag.
   */
  bool ShiftUp();

  /**
   * @brief Same moving image, (x,y+1) on both fixed and moving images.
   *
   * @returns Calculated flag.
   */
  bool ShiftDown();

  /**
   * @brief Same (x,y) coordinates on database[movingIndex - 1].
   *
   * @returns Calculated flag.
   */
  bool ShiftTop();

  /**
   * @brief Same (x,y) coordinates on database[movingIndex + 1].
   *
   * @returns Calculated flag.
   */
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
  bool PatchInsideImage(const size_t x, const size_t y);

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

template <class DatabaseType>
SSD<DatabaseType>::SSD(const DatabaseType &db, size_t idx,
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


template <class DatabaseType>
typename SSD<DatabaseType>::PixelType SSD<DatabaseType>::GetValue() const
{
  assert(calculated && "Value query from an invalid SSD object!");

  return value;
}


template <class DatabaseType>
SSD<DatabaseType>::operator PixelType() const
{
  return GetValue();
}


template <class DatabaseType>
SSD<DatabaseType>::operator bool() const
{
  return calculated;
}


template <class DatabaseType>
bool SSD<DatabaseType>::operator <(const SSD &other) const
{
  if (!calculated)
    return false;
  if (!other.calculated)
    return true;
  return value < other.value;
}


template <class DatabaseType>
bool SSD<DatabaseType>::ShiftRight()
{
  assert(calculated && "Shifting invalid SSD object!");

  // Boundaries checks.
  if (fixedTopLeftX + 1 + patchSide <= imageWidth &&
      movingTopLeftX + 1 + patchSide <= imageWidth) {
    for (size_t dy = 0; dy < patchSide; ++dy) {
      // Step to new place (1 px right the right side), add it.
      PixelType diff1 =
        (*fixedImage)(fixedTopLeftY + dy, fixedTopLeftX + patchSide) -
        (*movingImage)(movingTopLeftY + dy, movingTopLeftX + patchSide);

      // Step from old place, subtract it (the left side).
      PixelType diff2 =
        (*fixedImage)(fixedTopLeftY + dy, fixedTopLeftX) -
        (*movingImage)(movingTopLeftY + dy, movingTopLeftX);

      value += (diff1 * diff1 - diff2 * diff2);
    }
    ++fixedTopLeftX;
    ++movingTopLeftX;
  } else {
    calculated = false;
  }

  return calculated;
}


template <class DatabaseType>
bool SSD<DatabaseType>::ShiftLeft()
{
  assert(calculated && "Shifting invalid SSD object!");

  // Boundaries checks.
  if (fixedTopLeftX > 0 && movingTopLeftX > 0) {
    for (size_t dy = 0; dy < patchSide; ++dy) {
      // Step to new place (1 px left the left side), add it.
      PixelType diff1 =
        (*fixedImage)(fixedTopLeftY + dy, fixedTopLeftX - 1) -
        (*movingImage)(movingTopLeftY + dy, movingTopLeftX - 1);

      // Step from old place, subtract it (the right side).
      PixelType diff2 =
        (*fixedImage)(fixedTopLeftY + dy, fixedTopLeftX + patchSide - 1) -
        (*movingImage)(movingTopLeftY + dy, movingTopLeftX + patchSide - 1);

      value += (diff1 * diff1 - diff2 * diff2);
    }
    --fixedTopLeftX;
    --movingTopLeftX;
  } else {
    calculated = false;
  }

  return calculated;
}


template <class DatabaseType>
bool SSD<DatabaseType>::ShiftUp()
{
  assert(calculated && "Shifting invalid SSD object!");

  // Boundary checks.
  if (fixedTopLeftY > 0 && movingTopLeftY > 0) {
    for (size_t dx = 0; dx < patchSide; ++dx) {
      // Step to new place (1 px up the upper side), add it.
      PixelType diff1 =
        (*fixedImage)(fixedTopLeftY - 1, fixedTopLeftX + dx) -
        (*movingImage)(movingTopLeftY - 1, movingTopLeftX + dx);

      // Step from old place (the lower side), subtract it.
      PixelType diff2 =
        (*fixedImage)(fixedTopLeftY + patchSide - 1, fixedTopLeftX + dx) -
        (*movingImage)(movingTopLeftY + patchSide - 1, movingTopLeftX + dx);

      value += (diff1 * diff1 - diff2 * diff2);
    }
    --fixedTopLeftY;
    --movingTopLeftY;
  } else {
    calculated = false;
  }

  return calculated;
}


template <class DatabaseType>
bool SSD<DatabaseType>::ShiftDown()
{
  assert(calculated && "Shifting invalid SSD object!");

  // Boundary checks.
  if (fixedTopLeftY + 1 + patchSide <= imageHeight &&
      movingTopLeftY + 1 + patchSide <= imageHeight) {
    for (size_t dx = 0; dx < patchSide; ++dx) {
      // Step to new place (1 px down the lower side), add it.
      PixelType diff1 =
        (*fixedImage)(fixedTopLeftY + patchSide, fixedTopLeftX + dx) -
        (*movingImage)(movingTopLeftY + patchSide, movingTopLeftX + dx);

      // Step from old place (upper side), subtract it.
      PixelType diff2 =
        (*fixedImage)(fixedTopLeftY, fixedTopLeftX + dx) -
        (*movingImage)(movingTopLeftY, movingTopLeftX + dx);

      value += (diff1 * diff1 - diff2 * diff2);
    }
    ++fixedTopLeftY;
    ++movingTopLeftY;
  } else {
    calculated = false;
  }

  return calculated;
}


template <class DatabaseType>
bool SSD<DatabaseType>::ShiftTop()
{
  assert(calculated && "Shifting invalid SSD object!");

  if (movingIndex < 2) {
    // We're already at database[1], cannot go up the base.
    calculated = false;
  } else {
    --movingIndex;
    movingImage = &database.GetImage(movingIndex);

    CalculateValue();
  }

  return calculated;
}


template <class DatabaseType>
bool SSD<DatabaseType>::ShiftBottom()
{
  assert(calculated && "Shifting invalid SSD object!");

  if (movingIndex + 1 == database.GetImageCount()) {
    // We're at the last image in database, nowhere to go down.
    calculated = false;
  } else {
    ++movingIndex;
    movingImage = &database.GetImage(movingIndex);

    CalculateValue();
  }

  return calculated;
}


template <class DatabaseType>
void SSD<DatabaseType>::CalculateValue()
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


template <class DatabaseType>
bool SSD<DatabaseType>::PatchInsideImage(const size_t x, const size_t y)
{
  // x and y are coordinates of top-left corner of a patch.
  // Because of size_t type we don't need to check that x >= 0 and y >= 0.
  return (x + patchSide <= imageWidth) && (y + patchSide <= imageHeight);
}
