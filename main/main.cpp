#include "OPAL.h"
#include "ImageIO.h"
#include "SegmentationColorsConverter.h"
#include "SegmentationAccuracyEstimator.h"
#include <iostream>

int main(int argc, char **argv) {
  if (argc < 5)
    return 0;

  OPAL::DatabaseType db;
  db.Add(argv[1], argv[2]);
  db.Add(argv[3], argv[4]);

  OPAL opal(OPALSettings::GetDefaults(), db);
  opal.Run();

  auto seg = opal.GetOutput();

  ImageIO::SegmentationColorsConverter<OPAL::SegmentationPixelType> converter;

  auto rgb = converter.ConvertToRGB(seg);

  ImageIO::WriteImage(rgb, argv[5]);

  auto groundTruth = ImageIO::ReadImage<OPAL::SegmentationPixelType>(argv[6]);
  
  SegmentationAccuracyEstimator<OPAL::SegmentationPixelType> estimator;
  estimator.Estimate(groundTruth, seg);

  auto diceMap = estimator.GetDiceScoreMap();

  for (const auto &l : diceMap) {
    std::cout.width(3);
    std::cout << l.first << '\t' << l.second << std::endl;
  }

  return 0;
}
