#pragma once

#include <stdexcept>
#include <cassert>
#include <iostream>
#include <vector>
#include <string>


template <class T>
class Image {
public:
  using PixelType = T;

  Image() : Height(0), Width(0), data() {}

  // Creates a new image and fills it with \p value.
  Image(size_t h, size_t w, const T &value = T())
    : Height(h), Width(w), data(h * w, value)
  {
    if (!h || !w)
      Height = Width = 0;
  }

  Image(const Image<T> &other) = default;
  Image(Image<T> &&other) = default;

  virtual ~Image() = default;


  void Resize(size_t h, size_t w, const T &value = T()) {
    if (!h || !w)
      Height = Width = 0;
    else {
      Height = h;
      Width = w;
    }
    data.resize(Height * Width, value);
  }


  // Fill the whole image with the given value.
  void Fill(const T &value) {
    for (auto &pixel : data)
      pixel = value;
  }


  // Reads the input stream as plain text image.
  // Extension of the file name is NOT considered!
  //
  // Format:
  //   * 2 integers: height and width respectively;
  //   * <height>*<width> values separated by a whitespace.
  //
  // Throws sts::bad_alloc, std::invalid_argument, std ios exceptions.
  friend std::istream &operator>>(std::istream &is, Image<T> &m) {
    std::ios_base::sync_with_stdio(false);
    is.exceptions(std::ios_base::failbit | std::ios_base::badbit);

    is >> m.Height >> m.Width;
    m.data.resize(m.Height * m.Width);

    for (auto &pixel : m.data) {
      // If stream containes less data than declared - stop reading.
      if (!is.good())
        break;
      is >> pixel;
    }

    return is;
  }


  friend std::ostream &operator<<(std::ostream &os, const Image<T> &m) {
    std::ios_base::sync_with_stdio(false);
    os.exceptions(std::ios_base::failbit | std::ios_base::badbit);

    os << m.getHeight() << ' ' << m.getWidth() << '\n';
    for (size_t i = 0; i < m.getHeight(); ++i) {
      for (size_t j = 0; j < m.getWidth(); ++j) {
        os << m(i,j) << ' ';
      }
      os << '\n';
    }
    return os;
  }


  inline size_t getHeight() const { return Height; }
  inline size_t getWidth()  const { return Width;  }
  inline size_t getSize()   const {
    assert(data.size() == Height * Width &&
           "Image size is different than Height * Width!");
    return data.size();
  }


  inline bool isEmpty() const { return data.empty(); }


  Image<T> &operator=(Image<T> other) {
    swap(*this, other);
    return *this;
  }


  friend void swap(Image<T> &first, Image<T> &second) {
    using std::swap;
    swap(first.Height, second.Height);
    swap(first.Width, second.Width);
    swap(first.data, second.data);
  }


  template<class U>
  Image<U> castTo() const {
    Image<U> result(Height, Width);

    for (size_t i = 0; i < data.size(); ++i)
      result[i] = static_cast<U>(data[i]);

    return result;
  }


  bool operator==(const Image<T> &other) const {
    // Not actually needed check, but let's make this explicit for readability.
    if (isEmpty() != other.isEmpty())
      return false;
    if (Height != other.getHeight() || Width != other.getWidth())
      return false;

    for (size_t i = 0; i < data.size(); ++i)
      if (data[i] != other[i])
        return false;
    return true;
  }


  bool operator!=(const Image<T> &other) const {
    return !(*this == other);
  }


  Image<T> &operator+=(const Image<T> &other) {
    assert(Height == other.getHeight() && "Height mismatch!");
    assert(Width == other.getWidth() && "Width mismatch!");

    for (size_t i = 0; i < data.size(); ++i)
      data[i] += other[i];

    return *this;
  }


  Image<T> &operator-=(const Image<T> &other) {
    assert(Height == other.getHeight() && "Height mismatch!");
    assert(Width == other.getWidth() && "Width mismatch!");

    for (size_t i = 0; i < data.size(); ++i)
      data[i] -= other[i];

    return *this;
  }


  friend Image<T> operator+(Image<T> lhs, const Image<T> &rhs) {
    lhs += rhs;
    return lhs;
  }


  friend Image<T> operator-(Image<T> lhs, const Image<T> &rhs) {
    lhs -= rhs;
    return lhs;
  }


protected:
  size_t Height;
  size_t Width;
  std::vector<T> data;


public:
  auto operator()(size_t i, size_t j) -> decltype(data[0]) {
    assert(i < Height && "i index out of range!");
    assert(j < Width && "i index out of range!");

    return data[i * Width + j];
  }


  auto operator()(size_t i, size_t j) const -> decltype(data[0]) {
    assert(i < Height && "i index out of range!");
    assert(j < Width && "i index out of range!");

    return data[i * Width + j];
  }


  auto operator[](size_t i)       -> decltype(data.at(i)) { return data.at(i); }
  auto operator[](size_t i) const -> decltype(data.at(i)) { return data.at(i); }

  auto begin()       -> decltype(data.begin()) { return data.begin(); }
  auto begin() const -> decltype(data.begin()) { return data.begin(); }
  auto end()         -> decltype(data.end())   { return data.end();   }
  auto end()   const -> decltype(data.end())   { return data.end();   }
};

