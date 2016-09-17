#pragma once

#include "gtest/gtest.h"
#include "Matrix.h"

#include <fstream>
#include <type_traits> // std::enable_if

constexpr double EPS = 1.0e-5;

int RandomWithLimits(int lowerLimit, int upperLimit);

template<class T>
void FillRandomizedWithLimits(Matrix<T> &m, int lowerLimit, int upperLimit) {
  for (unsigned i = 0; i < m.getHeight(); ++i)
    for (unsigned j = 0; j < m.getWidth(); ++j)
      m[i][j] = static_cast<T>(RandomWithLimits(lowerLimit, upperLimit));
}

template<class T>
::testing::AssertionResult MatrixHasSize(const Matrix<T> &m,
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
::testing::AssertionResult MatrixIsEmpty(const Matrix<T> &m) {
  if (!m.isEmpty())
    return ::testing::AssertionFailure() << "isEmpty() returned false";
  return MatrixHasSize(m, 0, 0) << ", isEmpty() returned true";
}


// For integral types.
template<class T>
::testing::AssertionResult MatrixIsFilledWith(
  const Matrix<T> &m,
  const typename std::enable_if<std::is_integral<T>::value, T>::type &v)
{
  for (unsigned i = 0; i < m.getHeight(); ++i)
    for (unsigned j = 0; j < m.getWidth(); ++j)
      if (m[i][j] != v)
        return ::testing::AssertionFailure()
          << "value at (" << i << ", " << j << ") is not " << v;

  return ::testing::AssertionSuccess() << "matrix is filled with " << v;
}


template<class T>
::testing::AssertionResult MatrixIsFilledWith(
  const Matrix<T> &m,
  const typename std::enable_if<std::is_floating_point<T>::value, T>::type &v)
{
  for (unsigned i = 0; i < m.getHeight(); ++i)
    for (unsigned j = 0; j < m.getWidth(); ++j)
      if (std::abs(m[i][j] - v) > EPS)
        return ::testing::AssertionFailure()
          << "value at (" << i << ", " << j << ") is not " << v;

  return ::testing::AssertionSuccess() << "matrix is filled with " << v;
}
