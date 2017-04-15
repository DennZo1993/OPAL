/**
 * @file lib/OPAL.h
 *
 * @author Denis Zobnin
 *
 * @brief Header file with declaration of OPAL class.
 */


#pragma once

#include "ImageDatabase.h"
#include "OPALSettings.h"
#include "SSD.h"
#include "MaxVoteLabelEstimator.h"
#include "DummyLabelEstimator.h"

#include <chrono>
#include <random>
#include <functional>

/**
 * @brief Class implementing the core of OPAL algorithm.
 *
 * Class responsible for the whole OPAL algorithm. Takes a database of
 * images and a set of options to be constructed.
 *
 * Implements three main steps of OPAL:
 *   - Constrained initialization.
 *   - Iterative process
 *        1. Propagation step.
 *        2. Random search step.
 */
class OPAL {
public:
  // Fixed types.
  using ImgPixelType = double;
  using SegPixelType = int;
  using DatabaseType = ImageDatabase<ImgPixelType, SegPixelType>;
  using ImgType      = DatabaseType::ImgType;
  using SegType      = DatabaseType::SegType;
  using SSDType      = SSD<DatabaseType>;

  /**
   * @param [in] settings Set of OPAL options.
   * @param [in] database Set of input images and their segmentations.
   *                      Image to be segmented is database[0].
   */
  OPAL(const OPALSettings &settings, const DatabaseType &database);

  /**
   * @brief Constraint initialization.
   *
   * First step of OPAL algorithm.
   *
   * For each of the image located at (x, y) a random patch correspondence
   * located at {(x',y'), t'} where t' is the index of template in the library
   * is assigned. x' and y' are within the square initialization window around
   * (x, y).
   */
  void ConstrainedInitialization();


  /** @brief Propagation step with fast distance computation.
   *
   * Second step of OPAL algorithm. Performed on each iteration.
   *
   * For each patch consider its neighbors to determine if they provide a
   * better match.
   *
   * @param [in] iteration Index of iteration being performed.
   */
  void Propagation(size_t iteration);


  void BuildSegmentation();


  /// @return The result segmentation of input image.
  SegType GetOutput() const {
    return OutputSegmentation;
  }


  /**
   * @brief Run OPAL algorithm. Executes all stages successively.
   */
  void Run();


  /// @return X-coordinate offsets between nearest neighbor patches.
  const Image<int>& getFieldX() const { return FieldX; }

  /// @return Y-coordinate offsets between nearest neighbor patches.
  const Image<int>& getFieldY() const { return FieldY; }

  /// @return Indexes of images in database nearest neighbors belong to.
  const Image<size_t>& getFieldT() const { return FieldT; }

  /// @return Map of SSD values between patches.
  const Image<SSDType>& getSSDMap() const { return SSDMap; }


private:
  /// Global OPAL settings.
  const OPALSettings &Sets;

  /// Database of images. Image to be segmented at index 0.
  const DatabaseType &Database;

  // Output displacement fields.
  Image<int>     FieldX; ///< x-coordinate *offset*
  Image<int>     FieldY; ///< y-coordinate *offset*
  Image<size_t>  FieldT; ///< image index (in database)
  Image<SSDType> SSDMap; ///< SSD between patches with centers at (i,j).

  // Size of the images in the database. Just to replace call to
  // Database.getImageHeight(), Database.getImageWidth().
  size_t ImageHeight; ///< Height of images in database.
  size_t ImageWidth;  ///< Width of images in database.

  /**
   * @brief Input image to be segmented. Always Database[0].
   *
   * If offsetX = FieldX[i][j], offsetY = FieldY[i][j], T = FieldT[i][j],
   * then InputImage[i][j] = Database.getImage(T)[i+offsetY][j + offsetX]
   */
  ImgType InputImage;

  /// Result segmentation of input image.
  SegType OutputSegmentation;


  using RandomGeneratorType = std::mt19937;

  /// Pseudo-random generator for initialization step.
  RandomGeneratorType randGen;


  using FinalLabelEstimator      = DummyLabelEstimator<SegPixelType>;
  using CandidateLabelsContainer = FinalLabelEstimator::CandidateContainer;
  FinalLabelEstimator finalLabelEstimator;

private:
  int PropagatePixel(size_t i, size_t j, int delta);

  /// Save current displacement fields.
  void SaveCurrentFields(const std::string &fileName) const;

  /** @brief Calculate SSD at (i,j).
   *
   * Calculates SSD between input image and image with the index
   * FieldT(i,j) in the database.
   *
   * @param [in] i y-coordinate in the images.
   * @param [in] j x-coordinate in the images.
   */
  SSDType SSDAt(size_t i, size_t j) const;

  /// Recalculate the whole SSD map.
  void UpdateSSDMap();

  void GetCandidateLabelsForPixel(size_t i, size_t j,
                                  CandidateLabelsContainer &result) const;
};
