#pragma once

#include "Image.h"
#include <iostream>
#include <stdexcept>


template<class T>
class SSDObject {
public:
  SSDObject()
    : value(0)
    , calculated(false)
    , srcImage(nullptr)
    , srcI(0)
    , srcJ(0)
    , dstImage(nullptr)
    , dstI(0)
    , dstJ(0)
    , patchRadius(0)
    , patchSide(0)
  {}


  SSDObject(const Image<T> *src, size_t s_i, size_t s_j,
            const Image<T> *dst, size_t d_i, size_t d_j,
            size_t radius)
    : value(0)
    , calculated(false)
    , srcImage(src)
    , srcI(s_i)
    , srcJ(s_j)
    , dstImage(dst)
    , dstI(d_i)
    , dstJ(d_j)
    , patchRadius(radius)
    , patchSide(2 * radius + 1)
  {
    // If we try to construct a new SSDObject, we must be sure we pass
    // correct arguments to calculate it.
    assert(PatchInsideImage(srcImage, srcI, srcJ, patchRadius) &&
           "Patch is outside the source image!");
    assert(PatchInsideImage(dstImage, dstI, dstJ, patchRadius) &&
           "Patch is outside the destination image!");

    CalculateValue();

    // After construction we must have a valid SSDObject.
    assert(calculated && "SSD is not calculated after construction!");
  }

  SSDObject(const SSDObject &) = default;
  SSDObject(SSDObject &&) = default;
  ~SSDObject() = default;

  SSDObject& operator=(const SSDObject &) = default;
  SSDObject& operator=(SSDObject &&) = default;

public:
  T GetValue() const {
    assert(calculated && "Trying to get value of invalid SSDObject!");

    return value;
  }

  operator T() const { return GetValue(); }

  operator bool() const { return calculated; }

  bool operator<(const SSDObject &other) const {
    if (!calculated)
      return false;
    if (!other.calculated)
      return true;
    return value < other.value;
  }


  bool ShiftRight() {
    assert(calculated && "Shifting invalid SSDObject!");

    if (srcJ + 1 + patchRadius < srcImage->getWidth() &&
        dstJ + 1 + patchRadius < dstImage->getWidth()) {
      for (size_t di = 0; di < patchSide; ++di) {
        T diff1 = (*srcImage)(srcI + di - patchRadius, srcJ + patchRadius + 1) -
                  (*dstImage)(dstI + di - patchRadius, dstJ + patchRadius + 1);

        T diff2 = (*srcImage)(srcI + di - patchRadius, srcJ - patchRadius) -
                  (*dstImage)(dstI + di - patchRadius, dstJ - patchRadius);
        value += (diff1 * diff1 - diff2 * diff2);
      }
      ++srcJ;
      ++dstJ;
    } else {
      calculated = false;
    }

    return calculated;
  }


  bool ShiftLeft() {
    assert(calculated && "Shifting invalid SSDObject!");

    if (srcJ > patchRadius && dstJ > patchRadius) {
      for (size_t di = 0; di < patchSide; ++di) {
        T diff1 = (*srcImage)(srcI + di - patchRadius, srcJ - patchRadius - 1) -
                  (*dstImage)(dstI + di - patchRadius, dstJ - patchRadius - 1);

        T diff2 = (*srcImage)(srcI + di - patchRadius, srcJ + patchRadius) -
                  (*dstImage)(dstI + di - patchRadius, dstJ + patchRadius);
        value += (diff1 * diff1 - diff2 * diff2);
      }
      --srcJ;
      --dstJ;
    } else {
      calculated = false;
    }

    return calculated;
  }


  bool ShiftUp() {
    assert(calculated && "Shifting invalid SSDObject!");

    if (srcI > patchRadius && dstI > patchRadius) {
      for (size_t dj = 0; dj < patchSide; ++dj) {
        T diff1 = (*srcImage)(srcI - patchRadius - 1, srcJ + dj - patchRadius) -
                  (*dstImage)(dstI - patchRadius - 1, dstJ + dj - patchRadius);

        T diff2 = (*srcImage)(srcI + patchRadius, srcJ + dj - patchRadius) -
                  (*dstImage)(dstI + patchRadius, dstJ + dj - patchRadius);
        value += (diff1 * diff1 - diff2 * diff2);
      }
      --srcI;
      --dstI;
    } else {
      calculated = false;
    }

    return calculated;
  }


  bool ShiftDown() {
    assert(calculated && "Shifting invalid SSDObject!");

    if (srcI + 1 + patchRadius < srcImage->getHeight() &&
        dstI + 1 + patchRadius < dstImage->getHeight()) {
      for (size_t dj = 0; dj < patchSide; ++dj) {
        T diff1 = (*srcImage)(srcI + patchRadius + 1, srcJ + dj - patchRadius) -
                  (*dstImage)(dstI + patchRadius + 1, dstJ + dj - patchRadius);

        T diff2 = (*srcImage)(srcI - patchRadius, srcJ + dj - patchRadius) -
                  (*dstImage)(dstI - patchRadius, dstJ + dj - patchRadius);
        value += (diff1 * diff1 - diff2 * diff2);
      }
      ++srcI;
      ++dstI;
    } else {
      calculated = false;
    }

    return calculated;
  }


  friend std::ostream & operator<<(std::ostream &os, const SSDObject<T> &ssd) {
    if (!ssd.calculated) {
      os << "SSD not calculated!";
    } else {
      os << "SSD between ("
         << ssd.srcI << ", " << ssd.srcJ << ") and ("
         << ssd.dstI << ", " << ssd.dstJ << ") is: "
         << ssd.value << std::endl
         << std::endl;

      for (size_t di = 0; di < ssd.patchSide; ++di) {
        for (size_t dj = 0; dj < ssd.patchSide; ++dj) {
          os.width(3);
          os << (*ssd.srcImage)(ssd.srcI + di - ssd.patchRadius,
                                ssd.srcJ + dj - ssd.patchRadius)
             << ' ';
        }
        os << "     ";
        for (size_t dj = 0; dj < ssd.patchSide; ++dj) {
          os.width(3);
          os << (*ssd.dstImage)(ssd.dstI + di - ssd.patchRadius,
                                ssd.dstJ + dj - ssd.patchRadius)
             << ' ';
        }
        os << std::endl;
      }
      os << std::endl;
    }
    return os;
  }


private:
  static bool PatchInsideImage(const Image<T> *img, size_t i, size_t j,
                               size_t radius) {
    if (!img)
      return false;
    return i >= radius
        && j >= radius
        && i + radius < img->getHeight()
        && j + radius < img->getWidth();
  }


  void CalculateValue() {
    value = 0;

    for (size_t di = 0; di < patchSide; ++di)
      for (size_t dj = 0; dj < patchSide; ++dj) {
        T diff = (*srcImage)(srcI + di - patchRadius, srcJ + dj - patchRadius) -
                 (*dstImage)(dstI + di - patchRadius, dstJ + dj - patchRadius);
        value += diff * diff;
      }

    calculated = true;
  }


  T value;
  bool calculated;

  const Image<T> *srcImage;
  size_t srcI, srcJ;
  const Image<T> *dstImage;
  size_t dstI, dstJ;
  size_t patchRadius;
  size_t patchSide;
};
