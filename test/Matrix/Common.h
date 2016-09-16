#include "gtest/gtest.h"
#include "Matrix.h"
#include <fstream>
#include <cmath>    // fabs

constexpr double EPS = 1.0e-5;

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

template<class T>
::testing::AssertionResult MatrixIsFilledWith(const Matrix<T> &m,
                                              const T &value) {
  for (unsigned i = 0; i < m.getHeight(); ++i)
    for (unsigned j = 0; j < m.getWidth(); ++j)
      if (fabs(m[i][j] - value) > EPS)
        return ::testing::AssertionFailure()
          << "value at (" << i << ", " << j << ") is not " << value;

  return ::testing::AssertionSuccess() << "matrix is filled with " << value;
}
