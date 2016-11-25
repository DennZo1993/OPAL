#pragma once

#include "gtest/gtest.h"
#include "Image.h"

#include <fstream>
#include <type_traits> // std::enable_if
#include <random>

class RandGen {
public:
  static RandGen *GetInstance() {
    if (!instance)
      instance = new RandGen;
    return instance;
  }

  int Random(int lowerLimit, int upperLimit);
private:
  RandGen() {
    generator.seed(123);
  }

  RandGen(const RandGen &r) = delete;
  RandGen& operator=(const RandGen &r) = delete;

  static RandGen *instance;
  std::mt19937 generator;
};

#define RandomWithLimits(a, b) RandGen::GetInstance()->Random(a, b)

constexpr double EPS = 1.0e-5;

template<class T>
void FillRandomizedWithLimits(Image<T> &img, int lowerLimit, int upperLimit) {
  for (unsigned i = 0; i < img.getSize(); ++i)
    img[i] = static_cast<T>(RandomWithLimits(lowerLimit, upperLimit));
}

template<class T>
::testing::AssertionResult ImageHasSize(const Image<T> &m,
                                        unsigned h, unsigned w) {
  if (m.getHeight() != h)
    return ::testing::AssertionFailure()
      << "height " << m.getHeight() << " is not " << h;
  if (m.getWidth() != w)
    return ::testing::AssertionFailure()
      << "width " << m.getWidth() << " is not " << w;
  return ::testing::AssertionSuccess()
    << "height is " << h << ", width is " << w;
}

template<class T>
::testing::AssertionResult ImageIsEmpty(const Image<T> &m) {
  if (!m.isEmpty())
    return ::testing::AssertionFailure() << "isEmpty() returned false";
  return ImageHasSize(m, 0, 0) << ", isEmpty() returned true";
}


// For integral types.
template<class T>
::testing::AssertionResult ImageIsFilledWith(
  const Image<T> &m,
  const typename std::enable_if<std::is_integral<T>::value, T>::type &v)
{
  for (unsigned i = 0; i < m.getHeight(); ++i)
    for (unsigned j = 0; j < m.getWidth(); ++j)
      if (m(i, j) != v)
        return ::testing::AssertionFailure()
          << "value at (" << i << ", " << j << ") is not " << v;

  return ::testing::AssertionSuccess() << "matrix is filled with " << v;
}


template<class T>
::testing::AssertionResult ImageIsFilledWith(
  const Image<T> &m,
  const typename std::enable_if<std::is_floating_point<T>::value, T>::type &v)
{
  for (unsigned i = 0; i < m.getHeight(); ++i)
    for (unsigned j = 0; j < m.getWidth(); ++j)
      if (std::abs(m(i, j) - v) > EPS)
        return ::testing::AssertionFailure()
          << "value at (" << i << ", " << j << ") is not " << v;

  return ::testing::AssertionSuccess() << "matrix is filled with " << v;
}
