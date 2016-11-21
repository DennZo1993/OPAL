#include "flow2color.h"
#include <cmath>
#include <fstream>

static constexpr double PI = 3.14159265359;

Flow2Color::Flow2Color(const std::string &floFile) {
  FlowIO::ReadFlowFile(floFile, FlowX, FlowY);
  MakeColorWheel();
  rawPixels.resize(4 * FlowX.getHeight() * FlowX.getWidth());
}


Flow2Color::Flow2Color(const std::string &flowXFile, const std::string &flowYFile)
  : FlowX(flowXFile), FlowY(flowYFile)
{
  MakeColorWheel();
  rawPixels.resize(4 * FlowX.getHeight() * FlowX.getWidth());
}


void Flow2Color::MakeColorWheel() {
  // Relative lengths of color transitions: these are chosen based on perceptual similarity
  // (e.g. one can distinguish more shades between red and yellow than between yellow and green)
  size_t RY = 15;
  size_t YG = 6;
  size_t GC = 4;
  size_t CB = 11;
  size_t BM = 13;
  size_t MR = 6;

  size_t k = 0;
  for (size_t i = 0; i < RY; ++i)
    ColorWheel[k++] = RGBAPixel(255,	255*i/RY,	0);

  for (size_t i = 0; i < YG; ++i)
    ColorWheel[k++] = RGBAPixel(255-255*i/YG, 255, 0);

  for (size_t i = 0; i < GC; ++i)
    ColorWheel[k++] = RGBAPixel(0,	255, 255*i/GC);

  for (size_t i = 0; i < CB; ++i)
    ColorWheel[k++] = RGBAPixel(0, 255-255*i/CB, 255);

  for (size_t i = 0; i < BM; ++i)
    ColorWheel[k++] = RGBAPixel(255*i/BM, 0,	255);

  for (size_t i = 0; i < MR; ++i)
    ColorWheel[k++] = RGBAPixel(255,	0, 255-255*i/MR);
}


RGBAPixel Flow2Color::ComputeColorForFlowPixel(float fx, float fy) {
  float rad = sqrt(fx * fx + fy * fy);
  float a = atan2(-fy, -fx) / PI;
  float fk = (a + 1.0) / 2.0 * (NCOLORS-1);
  size_t k0 = static_cast<size_t>(fk);
  size_t k1 = (k0 + 1) % NCOLORS;
  float f = fk - k0;
  //f = 0; // uncomment to see original color wheel

  RGBAPixel result;

  for (size_t channel = 0; channel < 3; ++channel) {
    float col0 = ColorWheel[k0][channel] / 255.0;
    float col1 = ColorWheel[k1][channel] / 255.0;
    float col = (1 - f) * col0 + f * col1;
    if (rad <= 1)
        col = 1 - rad * (1 - col); // increase saturation with radius
    else
        col *= .75; // out of range
    result[channel] = static_cast<RGBAPixel::PixelType>(255.0 * col);
  }

  return result;
}


bool Flow2Color::UnknownFlow(float x, float y) {
  return (std::abs(x) > UNKNOWN_FLOW_THRESH)
      || (std::abs(y) > UNKNOWN_FLOW_THRESH)
      || std::isnan(x)
      || std::isnan(y);
}


void Flow2Color::Convert() {
  // determine motion range:
  float maxx = -999, maxy = -999;
  float minx =  999, miny =  999;
  float maxrad = -1;
  for (size_t y = 0; y < FlowX.getHeight(); ++y) {
    for (size_t x = 0; x < FlowX.getWidth(); ++x) {
      float fx = FlowX[y][x];
      float fy = FlowY[y][x];
      if (UnknownFlow(fx, fy))
        continue;
      maxx = std::max(maxx, fx);
      maxy = std::max(maxy, fy);
      minx = std::min(minx, fx);
      miny = std::min(miny, fy);
      float rad = sqrt(fx * fx + fy * fy);
      maxrad = std::max(maxrad, rad);
    }
  }

  std::cout << "Max motion: " << maxrad << "; motion range: "
            << "u = " << minx << " .. " << maxx << "; "
            << "v = " << miny << " .. " << maxy;

  for (size_t y = 0; y < FlowX.getHeight(); ++y) {
    for (size_t x = 0; x < FlowX.getWidth(); ++x) {
      float fx = FlowX[y][x];
      float fy = FlowY[y][x];
      RGBAPixel resPixel;
      if (!UnknownFlow(fx, fy))
        resPixel = ComputeColorForFlowPixel(fx/maxrad, fy/maxrad);

      rawPixels[4 * FlowX.getWidth() * y + 4 * x + 0] = resPixel[0];
      rawPixels[4 * FlowX.getWidth() * y + 4 * x + 1] = resPixel[1];
      rawPixels[4 * FlowX.getWidth() * y + 4 * x + 2] = resPixel[2];
      rawPixels[4 * FlowX.getWidth() * y + 4 * x + 3] = 255;
    }
  }
}


void Flow2Color::WriteToFile(const std::string &fileName) const {
  unsigned error = lodepng::encode(fileName, rawPixels, FlowX.getWidth(), FlowY.getHeight());
  (void) error;
}
