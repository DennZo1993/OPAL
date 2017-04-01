#pragma once

#include "MultipointLabelEstimator.h"


template <class LabelType>
class DummyLabelEstimator : public MultipointLabelEstimator<LabelType> {
public:

  using SuperClass = MultipointLabelEstimator<LabelType>;
  using CandidateContainer = typename SuperClass::CandidateContainer;
  using WeightType = typename SuperClass::WeightType;

  virtual LabelType
  EstimateLabel(const CandidateContainer &candidates) const override final {
    auto i = candidates.size() / 2;
    return candidates[i].first;
  }
};
