#pragma once

#include "MultipointLabelEstimator.h"
#include <unordered_map>
#include <algorithm>


template <class LabelType>
class MaxVoteLabelEstimator : public MultipointLabelEstimator<LabelType> {
public:

  using SuperClass = MultipointLabelEstimator<LabelType>;
  using CandidateContainer = typename SuperClass::CandidateContainer;
  using WeightType = typename SuperClass::WeightType;

  virtual LabelType
  EstimateLabel(const CandidateContainer &candidates) const override final {
    std::unordered_map<LabelType, WeightType> sumWeightMap;
    for (const auto &p : candidates) {
      sumWeightMap[p.first] += p.second;
    }

    auto maxIt =
      std::max_element(sumWeightMap.begin(), sumWeightMap.end(),
                       [](const std::pair<LabelType, WeightType> &f,
                          const std::pair<LabelType, WeightType> &s) -> bool {
                            return f.second < s.second;
                       });
    return maxIt->first;
  }
};
