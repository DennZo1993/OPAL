#pragma once

#include <vector>
#include <utility>


template <class LabelType>
class MultipointLabelEstimator {
public:
  using WeightType = double;

  // Candidate label and its weight (from 0 to 1).
  using CandidateType = std::pair<LabelType, WeightType>;

  using CandidateContainer = std::vector<CandidateType>;

  // Estimate final label by candidate labels with their weights.
  //
  // For example, each pixel contributes to NxN patches in PM algorithm
  // where N is patch side. Each of NxN patches votes for its own label.
  virtual LabelType
  EstimateLabel(const CandidateContainer &candidates) const = 0;
};
