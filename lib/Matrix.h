#pragma once

#include <cassert>
#include <cmath>      // std::abs
#include <iostream>
#include <algorithm>
#include <chrono>
#include <fstream>

#include "lodepng/lodepng.h"

template <class T>
class Matrix {
// ==========
// Constructors and destructor.
// ==========

public:

  Matrix() : Height(0), Width(0), data(nullptr) {}

  // Creates a new matrix and fills it with \p value.
  Matrix(int h, int w, const T &value) : data(nullptr) {
    AllocateAndFill(h, w, value);
  }

  // Copy constructor.
  Matrix(const Matrix<T> &other) : data(nullptr) {
    DeepCopyFrom(other);
  }

  // Move constructor.
  Matrix(Matrix<T> &&other) : Matrix<T>() {
    swap(*this, other);
  }

  // Construct new matrix and read it from file.
  Matrix(const std::string &fileName)
    : data(nullptr), Height(0), Width(0) {
    ReadFromFile(fileName);
  }

  ~Matrix() {
    Destroy();
  }

// ==========
// Allocation and destruction.
// ==========

public:

  // Destroy the Matrix and allocate data with new size.
  // Data will be uninitialized after this operation.
  void Allocate(int h, int w) {
    Destroy();
    // Negative size is a logical error.
    if (h < 0 || w < 0)
      // FIXME: this should be an exception!
      return;

    // Logically reallocation with either dimension equal to 0 is destruction.
    if (h == 0 || w == 0)
      return;

    Height = h;
    Width = w;

    data = new (std::nothrow) T*[Height];
    if (!data)
      return;
    for (unsigned i = 0; i < Height; ++i)
      data[i] = new (std::nothrow) T[Width];
  }

  void DeepCopyFrom(const Matrix<T> &other) {
    Allocate(other.getHeight(), other.getWidth());

    // We're possibly copying from an empty matrix. Then matrix must become
    // empty too. This is the only normal way to get empty.
    if (isEmpty()) {
      assert(other.isEmpty() && "Empty matrix after copy from non-empty one!");
      return;
    }

    assert(data && "Data is NULL!");
    for (unsigned i = 0; i < Height; ++i) {
      assert(data[i] && "Row data in NULL!");
      for (unsigned j = 0; j < Width; ++j)
        data[i][j] = other[i][j];
    }
  }

private:

  // Free allocated memory. Set dimensions to 0, data to nullptr.
  void Destroy() {
    if (data) {
      for (unsigned i = 0; i < Height; ++i)
        if (data[i])
          delete[] data[i];
      delete[] data;
      data = nullptr;
    }
    Height = Width = 0;
  }

// ==========
// Filling the values.
// ==========
//
// These functions should NOT allocate/free memory.

public:

  // Fill the whole matrix with the given value.
  void Fill(const T &value) {
    assert(((data && Height && Width) || (!data && !Height && !Width)) &&
           "Null data with non-zero dimensions or vice versa!");

    for (unsigned i = 0; i < Height; ++i) {
      assert(data[i] && "Row data in NULL!");

      for (unsigned j = 0; j < Width; ++j)
        data[i][j] = value;
    }
  }

  void AllocateAndFill(int h, int w, const T &value) {
    // This must be logically safe to just call Fill() as it must never fail:
    // if matrix gets empty after Allocate (<=> h and/or w is 0), Fill() must
    // do nothing.
    Allocate(h, w);
    Fill(value);
  }


// ==========
// Reading / writing.
// ==========
//
// Read functions reallocate memory.

public:

  // Reads the file as whitespace-separated plain text matrix.
  // Extension of the file name is NOT considered!
  //
  // File format:
  //   * The first row contains 2 integers: height and width respectively;
  //   * Next <height> lines contains <width> numbers each, separated by a
  //   * whitespace.
  void ReadFromPlainText(const std::string &fileName) {
    std::ifstream ifs(fileName);
    if (!ifs.good())
      return; // FIXME This should be an exception.
    ifs >> *this;
    ifs.close();
  }

  // Reads the file as PNG image.
  // Extension of the file name is NOT considered!
  void ReadFromPNG(const std::string &fileName) {
    std::vector<unsigned char> png;
    std::vector<unsigned char> image;
    unsigned width = 0, height = 0;

    if (lodepng::load_file(png, fileName))
      return; // FIXME: Throw an exception here.
    if (lodepng::decode(image, width, height, png))
      return; // FIXME: Throw an exception here.

    // Construct new Matrix. Convert RGB to grayscale.
    Allocate(height, width);
    for (unsigned y = 0; y < height; ++y) {
      for (unsigned x = 0; x < width; ++x) {
        auto R = image[4 * width * y + 4 * x + 0];
        auto G = image[4 * width * y + 4 * x + 1];
        auto B = image[4 * width * y + 4 * x + 2];
        auto gray = (R * 0.3) + (G * 0.59) + (B * 0.11);
        data[y][x] = gray;
      }
    }
  }

  // Based on file extension reads the file as plain text, PNG, etc.
  void ReadFromFile(const std::string &fileName) {
    if (fileNameHasExtension(fileName, ".png"))
      ReadFromPNG(fileName);
    else
      ReadFromPlainText(fileName);
  }

// Stream I/O.

  friend std::istream &operator >>(std::istream &is, Matrix<T> &m) {
    std::ios_base::sync_with_stdio(false);
    // Signed int to avoid reading negative numbers.
    int h = 0, w = 0;
    is >> h >> w;

    m.Allocate(h, w);

    for (unsigned i = 0; i < m.getHeight(); ++i)
      for (unsigned j = 0; j < m.getWidth(); ++j) {
        // If stream containes less data than declared - stop reading.
        if (!is.good())
          break;

        is >> m[i][j];
      }

    return is;
  }

  friend std::ostream &operator <<(std::ostream &os, const Matrix<T> &m) {
    std::ios_base::sync_with_stdio(false);

    os << m.getHeight() << ' ' << m.getWidth() << '\n';
    for (unsigned i = 0; i < m.getHeight(); ++i) {
      for (unsigned j = 0; j < m.getWidth(); ++j) {
        //os.width(3);
        os << m[i][j] << ' ';
      }
      os << '\n';
    }
    return os;
  }

// ==========
// Data accessors.
// ==========

public:

  T *&operator[](unsigned i) {
    assert(i < Height && "Row index out of bounds!");
    assert(data && "Subscript operation on null data!");
    return data[i];
  }

  const T *operator[](unsigned i) const {
    assert(i < Height && "Row index out of bounds!");
    assert(data && "Subscript operation on null data!");
    return data[i];
  }

  inline unsigned getHeight() const { return Height; }
  inline unsigned getWidth() const { return Width; }

  inline bool isEmpty() const {
    assert(((!data && !Height && !Width) || (data && Height && Width)) &&
           "Either non-zero size with empty data or vice versa!");
    return data == nullptr;
  }

// ==========
// Assignment / cast.
// ==========

public:

  Matrix<T> &operator=(Matrix<T> other) {
    swap(*this, other);
    return *this;
  }

  friend void swap(Matrix<T> &first, Matrix<T> &second) {
    using std::swap;
    swap(first.Height, second.Height);
    swap(first.Width, second.Width);
    swap(first.data, second.data);
  }

  // Cast.
  template<class U>
  Matrix<U> castTo() const {
    Matrix<U> result(Height, Width, 0);

    for (unsigned i = 0; i < Height; ++i)
      for (unsigned j = 0; j < Width; ++j)
        result[i][j] = static_cast<U>(data[i][j]);

    return result;
  }

// ==========
// Comparison operators.
// ==========
  bool operator==(const Matrix<T> &other) const {
    // Not actually needed check, but let's make this explicit for readability.
    if (isEmpty() != other.isEmpty())
      return false;
    if (Height != other.getHeight() || Width != other.getWidth())
      return false;

    for (unsigned i = 0; i < Height; ++i)
      for (unsigned j = 0; j < Width; ++j)
        if (data[i][j] != other[i][j])
          return false;
    return true;
  }

  bool operator!=(const Matrix<T> &other) const {
    return !(*this == other);
  }

// ==========
// Arithmetic operators.
// ==========

  Matrix<T> &operator+=(const Matrix<T> &other) {
    assert(Height == other.getHeight() && "Matrices have different heights!");
    assert(Width == other.getWidth() && "Matrices have different widths!");
    // If the matrix is empty, the other is empty too.
    assert(isEmpty() == other.isEmpty() && "isEmpty() works incorrectly!");
    if (isEmpty())
      return *this;

    for (unsigned i = 0; i < Height; ++i)
      for (unsigned j = 0; j < Width; ++j)
        data[i][j] += other[i][j];
    return *this;
  }

  Matrix<T> &operator-=(const Matrix<T> &other) {
    assert(Height == other.getHeight() && "Matrices have different heights!");
    assert(Width == other.getWidth() && "Matrices have different widths!");
    // If the matrix empty, the other is empty too.
    assert(isEmpty() == other.isEmpty() && "isEmpty() works incorrectly!");
    if (isEmpty())
      return *this;

    for (unsigned i = 0; i < Height; ++i)
      for (unsigned j = 0; j < Width; ++j)
        data[i][j] -= other[i][j];
    return *this;
  }

  friend Matrix<T> operator+(Matrix<T> lhs, const Matrix<T> &rhs) {
    lhs += rhs;
    return lhs;
  }

  friend Matrix<T> operator-(Matrix<T> lhs, const Matrix<T> &rhs) {
    lhs -= rhs;
    return lhs;
  }


  // SSD between whole matrices.
  T SSD(const Matrix<T> &other) {
    assert(Height == other.getHeight() && "Matrices have different heights!");
    assert(Width == other.getWidth() && "Matrices have different widths!");
    T result = 0;
    for (unsigned int i = 0; i < Height; ++i)
      for (unsigned int j = 0; j < Width; ++j) {
        T diff = data[i][j] - other[i][j];
        result += diff * diff;
      }

    assert(result >= 0 && "SSD cannot be less than 0!");
    return result;
  }

  // SSD between patches.
  T SSD(const Matrix<T> &other,
        unsigned i1, unsigned j1, unsigned i2, unsigned j2,
        int patchRadX, int patchRadY) {
    // Make sure index won't get out of range.
    assert(i1 >= patchRadY && i2 >= patchRadY &&
           i1 + patchRadY < Height && i2 + patchRadY < other.getHeight());
    assert(j1 >= patchRadX && j2 >= patchRadX &&
           j1 + patchRadX < Width && j2 + patchRadX < other.getWidth());
    assert(patchRadX > 0 && patchRadY > 0);

    T result = 0;
    for (int dy = -1 * patchRadY; dy <= patchRadY; ++dy)
      for (int dx = -1 * patchRadX; dx <= patchRadX; ++dx) {
        T diff = data[i1 + dy][j1 + dx] - other[i2 + dy][j2 + dx];
        result += diff * diff;
      }
    assert(result >= 0 && "SSD cannot be less than 0!");
    return result;
  }

  // Returns known SSD shifted 1 pixel right.
  // knownSSD = SSD(other, i1, j1, i2, j2, patchRadX, patchRadY).
  T SSD_ShiftedRight(const Matrix<T> &other,
                     unsigned i1, unsigned j1, unsigned i2, unsigned j2,
                     int patchRadX, int patchRadY, T knownSSD) {
#ifndef NDEBUG
    assert(i1 >= patchRadY && i2 >= patchRadY &&
           i1 + patchRadY < Height && i2 + patchRadY < other.getHeight());
    assert(j1 >= patchRadX && j2 >= patchRadX &&
           j1 + patchRadX + 1 < Width && j2 + patchRadX + 1 < other.getWidth());
    assert(patchRadX > 0 && patchRadY > 0);
    auto trustedKnownSSD = SSD(other, i1, j1, i2, j2, patchRadX, patchRadY);
    assert(std::abs(trustedKnownSSD - knownSSD) < 1e-5 && "knownSSD is out-of-date!");
#endif

    T result = knownSSD;
    for (int dy = -1 * patchRadY; dy <= patchRadY; ++dy) {
      // Positive.
      T diff1 = data[i1 + dy][j1 + patchRadX + 1] - other[i2 + dy][j2 + patchRadX + 1];
      // Negative.
      T diff2 = data[i1 + dy][j1 - patchRadX] - other[i2 + dy][j2 - patchRadX];
      result = result + diff1 * diff1 - diff2 * diff2;
    }
    assert(result >= 0 && "SSD cannot be less than 0!");
    return result;
  }

  // Returns known SSD shifted 1 pixel left.
  T SSD_ShiftedLeft(const Matrix<T> &other,
                    unsigned i1, unsigned j1, unsigned i2, unsigned j2,
                    int patchRadX, int patchRadY, T knownSSD) {
#ifndef NDEBUG
    assert(i1 >= patchRadY && i2 >= patchRadY &&
           i1 + patchRadY < Height && i2 + patchRadY < other.getHeight());
    assert(j1 >= patchRadX + 1 && j2 >= patchRadX + 1 &&
           j1 + patchRadX < Width && j2 + patchRadX < other.getWidth());
    assert(patchRadX > 0 && patchRadY > 0);
    auto trustedKnownSSD = SSD(other, i1, j1, i2, j2, patchRadX, patchRadY);
    assert(std::abs(trustedKnownSSD - knownSSD) < 1e-5 && "knownSSD is out-of-date!");
#endif

    T result = knownSSD;
    for (int dy = -1 * patchRadY; dy <= patchRadY; ++dy) {
      // Positive.
      T diff1 = data[i1 + dy][j1 - patchRadX - 1] - other[i2 + dy][j2 - patchRadX - 1];
      // Negative.
      T diff2 = data[i1 + dy][j1 + patchRadX] - other[i2 + dy][j2 + patchRadX];
      result = result + diff1 * diff1 - diff2 * diff2;
    }
    assert(result >= 0 && "SSD cannot be less than 0!");
    return result;
  }

  // Returns known SSD shifted 1 pixel up.
  // knownSSD = SSD(other, i1, j1, i2, j2, patchRadX, patchRadY).
  T SSD_ShiftedUp(const Matrix<T> &other,
                  unsigned i1, unsigned j1, unsigned i2, unsigned j2,
                  int patchRadX, int patchRadY, T knownSSD) {
#ifndef NDEBUG
    assert(i1 >= patchRadY + 1 && i2 >= patchRadY + 1 &&
           i1 + patchRadY < Height && i2 + patchRadY < other.getHeight());
    assert(j1 >= patchRadX && j2 >= patchRadX &&
           j1 + patchRadX < Width && j2 + patchRadX < other.getWidth());
    assert(patchRadX > 0 && patchRadY > 0);
    auto trustedKnownSSD = SSD(other, i1, j1, i2, j2, patchRadX, patchRadY);
    assert(std::abs(trustedKnownSSD - knownSSD) < 1e-5 && "knownSSD is out-of-date!");
#endif

    T result = knownSSD;
    for (int dx = -1 * patchRadX; dx <= patchRadX; ++dx) {
      // Positive.
      T diff1 = data[i1 - patchRadY - 1][j1 + dx] - other[i2 - patchRadY - 1][j2 + dx];
      // Negative.
      T diff2 = data[i1 + patchRadY][j1 + dx] - other[i2 + patchRadY][j2 + dx];
      result = result + diff1 * diff1 - diff2 * diff2;
    }
    assert(result >= 0 && "SSD cannot be less than 0!");
    return result;
  }

  // Returns known SSD shifted 1 pixel down.
  // knownSSD = SSD(other, i1, j1, i2, j2, patchRadX, patchRadY).
  T SSD_ShiftedDown(const Matrix<T> &other,
                    unsigned i1, unsigned j1, unsigned i2, unsigned j2,
                    int patchRadX, int patchRadY, T knownSSD) {
#ifndef NDEBUG
    assert(i1 >= patchRadY && i2 >= patchRadY &&
           i1 + patchRadY + 1 < Height && i2 + patchRadY + 1 < other.getHeight());
    assert(j1 >= patchRadX && j2 >= patchRadX &&
           j1 + patchRadX < Width && j2 + patchRadX < other.getWidth());
    assert(patchRadX > 0 && patchRadY > 0);
    auto trustedKnownSSD = SSD(other, i1, j1, i2, j2, patchRadX, patchRadY);
    assert(std::abs(trustedKnownSSD - knownSSD) < 1e-5 && "knownSSD is out-of-date!");
#endif

    T result = knownSSD;
    for (int dx = -1 * patchRadX; dx <= patchRadX; ++dx) {
      // Positive.
      T diff1 = data[i1 + patchRadY + 1][j1 + dx] - other[i2 + patchRadY + 1][j2 + dx];
      // Negative.
      T diff2 = data[i1 - patchRadY][j1 + dx] - other[i2 - patchRadY][j2 + dx];
      result = result + diff1 * diff1 - diff2 * diff2;
    }
    assert(result >= 0 && "SSD cannot be less than 0!");
    return result;
  }



  void PRINT_SSD(const Matrix<T> &other,
                 unsigned i1, unsigned j1, unsigned i2, unsigned j2,
                 int patchRadX, int patchRadY = 0) {
    if (!patchRadY)
      patchRadY = patchRadX;
    assert(i1 >= patchRadY && i2 >= patchRadY &&
      i1 + patchRadY < Height && i2 + patchRadY < other.getHeight());
    assert(j1 >= patchRadX && j2 >= patchRadX &&
      j1 + patchRadX < Width && j2 + patchRadX < other.getWidth());
    assert(patchRadX > 0 && patchRadY > 0);
    T result = 0;
    for (int dy = -1 * patchRadY; dy <= patchRadY; ++dy) {
      for (int dx = -1 * patchRadX; dx <= patchRadX; ++dx) {
        std::cout.width(4);
        std::cout << other[i2 + dy][j2 + dx] << ' ';
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
  }

private:

  // Returns true if filename /p source has extension /p ext.
  // Precondition: ext must start with '.'
  static bool fileNameHasExtension(const std::string &source,
                                   const std::string &ext) {
    assert(ext.length() > 0 && "Extension is empty!");
    assert(ext[0] == '.' && "Extension must start with '.' !");
    // source must represent a valid (non-empty) file name.
    // So at least 1 character + ext.
    if (source.length() < ext.length() + 1)
      return false;
    return source.substr(source.length() - ext.length(), ext.length()) == ext;
  }

  unsigned Height;
  unsigned Width;
  T **data;
};

