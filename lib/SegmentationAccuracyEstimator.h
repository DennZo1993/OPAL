#pragma once

#include "Image.h"

#include <stdexcept>
#include <map>


template <class T, class Enable = void>
class SegmentationAccuracyEstimator;

template <class T>
class SegmentationAccuracyEstimator<
        T, typename std::enable_if<std::is_integral<T>::value>::type>
{
public:
  SegmentationAccuracyEstimator() = default;
  ~SegmentationAccuracyEstimator() = default;
  
  void Estimate(const Image<T> &src, const Image<T> &dst) {
    if (src.getWidth() != dst.getWidth() || src.getHeight() != dst.getHeight())
      throw std::invalid_argument("Image dimensions mismatch!");
    
    Clear();
    
    for (size_t i = 0; i < src.getSize(); ++i) {
      auto srcLabel = src[i];
      auto dstLabel = dst[i];
      DiceScoresPerLabel[srcLabel] = DiceScoresPerLabel[dstLabel] = 0;
      ++SrcPixelPerLabelCount[srcLabel];
      ++DstPixelPerLabelCount[dstLabel];
      if (srcLabel == dstLabel)
        ++BothPixelPerLabelCount[srcLabel];
    }
    
    for (const auto &l : BothPixelPerLabelCount) {
      auto A = SrcPixelPerLabelCount.at(l.first);
      auto B = DstPixelPerLabelCount.at(l.first);
      DiceScoresPerLabel.at(l.first) = double(2 * l.second) / double(A + B);
    }
  }
  
  std::map<T, double> GetDiceScoreMap() const { return DiceScoresPerLabel; }

private:
  void Clear() {
    SrcPixelPerLabelCount.clear();
    DstPixelPerLabelCount.clear();
    BothPixelPerLabelCount.clear();
    DiceScoresPerLabel.clear();
  }

  std::map<T, size_t> SrcPixelPerLabelCount;
  std::map<T, size_t> DstPixelPerLabelCount;
  std::map<T, size_t> BothPixelPerLabelCount;
  std::map<T, double> DiceScoresPerLabel;
};
