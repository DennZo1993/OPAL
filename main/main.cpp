/**
 * \file main/main.cpp
 *
 * @author Denis Zobnin
 *
 * @brief Entry point of OPAL application.
 *
 * Defines the entry point of OPAL application. All main logic is prepared and
 * called from here.
 *
 *
 * @mainpage OPAL Main Page
 *
 * Optimized PatchMatch
 *
 * <B>Original paper: </B>
 * <I>Vinh-Thong Ta, R´emi Giraud, D. Louis Collins, Pierrick Coup´e.
 * Optimized PatchMatch for Near Real Time and Accurate Label Fusion.
 * MICCAI 2014, Sep 2014, United States. 8 p., 2014.</I>
 */


 // Pushystique
 
#include "OPAL.h"
#include "ImageIO.h"
#include "SegmentationColorsConverter.h"
#include "SegmentationAccuracyEstimator.h"

#include <iostream>
#include <ctime>


int main(int argc, char **argv) {
  OPALSettings settings = OPALSettings::ReadFromFile(argv[1]);

  std::cout << settings << std::endl;

  std::cout << "Database:" << std::endl;

  OPAL::DatabaseType db;
  for (int i = 2; i < argc - 2; i += 2) {
    db.Add(argv[i], argv[i+1]);
    std::cout << argv[i] << "   " << argv[i+1] << std::endl;
  }

  std::string resultDir(argv[argc-1]);

  std::cout << "\nresult dir: " << resultDir << std::endl;
  std::cout << "gt: " << argv[3] << std::endl;

  OPAL opal(settings, db);

  auto clocksBefore = clock();

  opal.Run();

  auto clocksAfter = clock();
  double timeConsumed = double(clocksAfter - clocksBefore) / double(CLOCKS_PER_SEC);

  auto seg = opal.GetOutput();
  auto groundTruth = ImageIO::ReadImage<OPAL::SegPixelType>(argv[3]);

  ImageIO::SegmentationColorsConverter converter;

  auto rgbResult = converter.ConvertToRGB(db.GetImage(0), seg);
  auto rgbGT = converter.ConvertToRGB(db.GetImage(0), groundTruth);

  ImageIO::WriteImage(rgbResult, resultDir + "/result.png");
  ImageIO::WriteImage(rgbGT, resultDir + "/ground_truth.png");

  SegmentationAccuracyEstimator<OPAL::SegPixelType> estimator;
  estimator.Estimate(groundTruth, seg);

  auto diceMap = estimator.GetDiceScoreMap();

  std::cout << "\nDice scores:" << std::endl;
  for (const auto &l : diceMap) {
    std::cout.width(3);
    std::cout << l.first << '\t' << l.second << std::endl;
  }

  std::cout << "\nOPAL running time: " << timeConsumed << std::endl;

  return 0;
}
