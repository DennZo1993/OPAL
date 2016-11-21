#pragma once

#include "lodepng/lodepng.h"
#include "../FloFileIO.h"
#include "../../lib/ImageTypes.h"
#include <stdexcept>
#include <vector>
#include <array>
#include <iostream>


class Flow2Color {
public:
  Flow2Color(const std::string &floFile);
  Flow2Color(const std::string &flowXFile, const std::string &flowYFile);
  ~Flow2Color() = default;

  void Convert();
  void WriteToFile(const std::string &fileName) const;

private:
  void MakeColorWheel();
  RGBAPixel ComputeColorForFlowPixel(float fx, float fy);
  static bool UnknownFlow(float x, float y);

  // Number of colors in the color wheel.
  static constexpr size_t NCOLORS = 55;
  static constexpr double UNKNOWN_FLOW_THRESH = 1e9;
  static constexpr double UNKNOWN_FLOW = 1e10;

  Matrix<float> FlowX;
  Matrix<float> FlowY;
  std::array<RGBAPixel, NCOLORS> ColorWheel;
  RGBImage rgbImage;
};
