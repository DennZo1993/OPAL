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

TEST(LabelEstimators, Test2) {
  using EstimatorType = MaxVoteLabelEstimator<int>;
  EstimatorType::CandidateContainer candidates = {
    { 41, 1.0 },
    { 43, 1.0 },
    { 43, 1.0 },
    { 43, 1.0 },
    {  0, 1.0 },
    { 41, 1.0 },
    { 43, 1.0 },
    { 43, 1.0 },
    {  0, 1.0 },
    {  0, 1.0 },
    {  0, 1.0 },
    {  0, 1.0 },
    {  0, 1.0 },
    {  0, 1.0 },
    {  0, 1.0 },
    {  0, 1.0 },
    {  0, 1.0 },
    {  0, 1.0 },
    {  0, 1.0 },
    {  0, 1.0 },
    { 49, 1.0 },
    {  0, 1.0 },
    {  0, 1.0 },
    {  0, 1.0 },
    {  0, 1.0 }
  };

  EstimatorType estimator;

  ASSERT_EQ(0, estimator.EstimateLabel(candidates));
}
