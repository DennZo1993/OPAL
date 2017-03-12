#include "MaxVoteLabelEstimator.h"
#include "gtest/gtest.h"


TEST(LabelEstimators, Test1) {
  using EstimatorType = MaxVoteLabelEstimator<int>;
  EstimatorType::CandidateContainer candidates = {
    { 1, 3 },
    { 3, 2 },
    { 2, 2 },
    { 3, 3 },
    { 2, 1 },
    { 3, 2 }
  };

  EstimatorType estimator;

  ASSERT_EQ(3, estimator.EstimateLabel(candidates));
}
