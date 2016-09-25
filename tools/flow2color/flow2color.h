#pragma once

#include "lodepng/lodepng.h"
#include "../FloFileIO.h"
#include <stdexcept>
#include <vector>
#include <iostream>


struct RGBPixel {
  using PixelType = unsigned char;
  RGBPixel(PixelType _r = 0, PixelType _g = 0, PixelType _b = 0)
    : r(_r), g(_g), b(_b) {}

  PixelType& operator[](size_t i) {
    switch (i) {
    case 0: return r;
    case 1: return g;
    case 2: return b;
    default: throw std::invalid_argument("Color channel index is invalid!");
    }
  }

  const PixelType& operator[](size_t i) const {
    switch (i) {
    case 0: return r;
    case 1: return g;
    case 2: return b;
    default: throw std::invalid_argument("Color channel index is invalid!");
    }
  }

  friend std::ostream& operator<<(std::ostream &os, const RGBPixel &p) {
    os << '(' << (int)p.r << ',' << (int)p.g << ',' << (int)p.b << ')';
    return os;
  }

  PixelType r;
  PixelType g;
  PixelType b;
};


class Flow2Color {
public:
  Flow2Color(const std::string &floFile);
  Flow2Color(const std::string &flowXFile, const std::string &flowYFile);
  ~Flow2Color() = default;

  void Convert();
  void WriteToFile(const std::string &fileName) const;

private:
  void MakeColorWheel();
  RGBPixel ComputeColorForFlowPixel(float fx, float fy);
  static bool UnknownFlow(float x, float y);

  // Number of colors in the color wheel.
  static constexpr size_t NCOLORS = 55;
  static constexpr double UNKNOWN_FLOW_THRESH = 1e9;
  static constexpr double UNKNOWN_FLOW = 1e10;

  Matrix<float> FlowX;
  Matrix<float> FlowY;
  RGBPixel ColorWheel[NCOLORS];
  std::vector<RGBPixel::PixelType> rawPixels;
};