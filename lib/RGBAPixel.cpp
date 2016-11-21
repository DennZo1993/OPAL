#include "RGBAPixel.h"


const RGBAPixel RGBAPixel::PureBlack = RGBAPixel(0  , 0  , 0  , 255);
const RGBAPixel RGBAPixel::PureWhite = RGBAPixel(255, 255, 255, 255);
const RGBAPixel RGBAPixel::PureRed   = RGBAPixel(255, 0  , 0  , 255);
const RGBAPixel RGBAPixel::PureGreen = RGBAPixel(0  , 255, 0  , 255);
const RGBAPixel RGBAPixel::PureBlue  = RGBAPixel(0  , 0  , 255, 255);


RGBAPixel::PixelType& RGBAPixel::operator[](size_t i) {
  switch (i) {
  case 0: return r;
  case 1: return g;
  case 2: return b;
  case 3: return a;
  default: throw std::invalid_argument("Color channel index is invalid!");
  }
}


const RGBAPixel::PixelType& RGBAPixel::operator[](size_t i) const {
  switch (i) {
  case 0: return r;
  case 1: return g;
  case 2: return b;
  case 3: return a;
  default: throw std::invalid_argument("Color channel index is invalid!");
  }
}


std::istream& operator>>(std::istream &is, RGBAPixel &p) {
  is >> p.r >> p.g >> p.b >> p.a;
  return is;
}


std::ostream& operator<<(std::ostream &os, const RGBAPixel &p) {
  os << '(' << (int)p.r << ',' << (int)p.g << ',' << (int)p.b
     << ',' << (int)p.a << ')';
  return os;
}


bool RGBAPixel::operator==(const RGBAPixel &other) const {
  return r == other.r
      && g == other.g
      && b == other.b
      && a == other.a;
}


bool RGBAPixel::operator!=(const RGBAPixel &other) const {
  return !(*this == other);
}


RGBAPixel& RGBAPixel::operator+=(const RGBAPixel &other) {
  r += other.r;
  g += other.g;
  b += other.b;
  a += other.a;
  return *this;
}


RGBAPixel& RGBAPixel::operator-=(const RGBAPixel &other) {
  r -= other.r;
  g -= other.g;
  b -= other.b;
  a -= other.a;
  return *this;
}


RGBAPixel operator+(RGBAPixel lhs, const RGBAPixel &rhs) {
  lhs += rhs;
  return lhs;
}


RGBAPixel operator-(RGBAPixel lhs, const RGBAPixel &rhs) {
  lhs -= rhs;
  return lhs;
}

