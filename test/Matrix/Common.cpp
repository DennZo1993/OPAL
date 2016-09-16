#include "Common.h"

int RandomWithLimits(int lowerLimit, int upperLimit) {
  assert(upperLimit > lowerLimit);
  using namespace std::chrono;
  time_point<system_clock, microseconds> seed =
    time_point_cast<microseconds>(system_clock::now());
  srand(seed.time_since_epoch().count());
  assert(upperLimit > lowerLimit);
  int limitDiff = upperLimit - lowerLimit;
  return rand() % limitDiff + lowerLimit;
}

