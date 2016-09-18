#include "Common.h"

RandGen *RandGen::instance = nullptr;

int RandGen::Random(int lowerLimit, int upperLimit) {
  assert(upperLimit > lowerLimit);

  std::uniform_int_distribution<int> dist(lowerLimit, upperLimit);
  return dist(generator);
}

