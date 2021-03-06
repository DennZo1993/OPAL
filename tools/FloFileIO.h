#pragma once

#include "../lib/Image.h"
#include <fstream>
#include <stdexcept>
#include <cmath>

namespace FlowIO {

static constexpr float TAG_FLOAT = 202021.25f;
static constexpr size_t BYTES_COUNT = 4;
static constexpr float EPS = 0.0001f;

template<class T>
void ReadFlowFile(const std::string &fileName,
                  Image<T> &FlowX, Image<T> &FlowY) {
  std::ifstream ifs;
  ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  ifs.open(fileName, std::ios::binary);

  int width, height;
  float tag;

  ifs.read(reinterpret_cast<char*>(&tag), BYTES_COUNT);
  if (std::abs(tag - TAG_FLOAT) > EPS)
    throw std::runtime_error("Wrong TAG read from file " + fileName);

  ifs.read(reinterpret_cast<char*>(&width), BYTES_COUNT);
  ifs.read(reinterpret_cast<char*>(&height), BYTES_COUNT);

  FlowX.Allocate(height, width);
  FlowY.Allocate(height, width);

  for (int i = 0; i < height; ++i)
    for (int j = 0; j < width; ++j) {
      float x = 0.0f, y = 0.0f;
      ifs.read(reinterpret_cast<char*>(&x), BYTES_COUNT);
      ifs.read(reinterpret_cast<char*>(&y), BYTES_COUNT);
      FlowX(i, j) = static_cast<T>(x);
      FlowY(i, j) = static_cast<T>(y);
    }
}


template<class T>
void WriteFlowFile(const std::string &fileName,
                   const Image<T> &FlowX, const Image<T> &FlowY) {
  if (FlowX.isEmpty() || FlowY.isEmpty())
    throw std::invalid_argument("One of the matrices is empty!");
  if (FlowX.getHeight() != FlowY.getHeight())
    throw std::invalid_argument("Matrices have different heights!");
  if (FlowX.getWidth() != FlowY.getWidth())
    throw std::invalid_argument("Matrices have different widths!");

  std::ofstream ofs;
  ofs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  ofs.open(fileName, std::ios::binary);

  // Write the tag.
  ofs.write(reinterpret_cast<const char*>(&TAG_FLOAT), BYTES_COUNT);

  // Write the size.
  int width = static_cast<int>(FlowX.getWidth());
  int height = static_cast<int>(FlowX.getHeight());
  ofs.write(reinterpret_cast<char*>(&width), BYTES_COUNT);
  ofs.write(reinterpret_cast<char*>(&height), BYTES_COUNT);

  for (size_t i = 0; i < FlowX.getHeight(); ++i)
    for (size_t j = 0; j < FlowX.getWidth(); ++j) {
      float xFlow = static_cast<float>(FlowX(i, j));
      float yFlow = static_cast<float>(FlowY(i, j));
      ofs.write(reinterpret_cast<char*>(&xFlow), BYTES_COUNT);
      ofs.write(reinterpret_cast<char*>(&yFlow), BYTES_COUNT);
    }
}

} // FlowIO namespace

