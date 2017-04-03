/**
 * @file lib/OPALSettings.h
 *
 * @author Denis Zobnin
 *
 * @brief Header with declaration of OPALSettings class.
 *
 * OPALSettings contains various OPAL options and handles reading config file.
 */


#pragma once

#include <string>
#include <map>


/**
 * @brief A lightweight class containing various OPAL options.
 */
struct OPALSettings {
private:
  /**
   * @brief Constructor from a complete bunch of parameters.
   */
  OPALSettings(size_t _initWindowRadius,
               size_t _patchRadius,
               bool _intermediateSaving,
               const std::string &_savingPath,
               size_t _maxIter);

  /**
   * @brief Constructor from set of settings.
   *
   * @param [in] sets Set of settings.
   */
  OPALSettings(const std::map<std::string, std::string> &sets);

  /**
   * @returns Default values for all settings.
   */
  static std::map<std::string, std::string> GetDefaultSetsMap();

public:
  /**
   * @returns Default values for all settings.
   */
  static OPALSettings GetDefaults();

  /**
   * @brief Read settings from plain text file.
   *
   * Values are separated by '='.
   *
   * @param [in] fileName Name of settings file.
   */
  static OPALSettings ReadFromFile(const std::string &fileName);

  /**
   * @brief Plain text dumping of all parameters to output stream.
   *
   * @param [out] os Output stream.
   * @param [in] sets OPAL settings to dump.
   */
  friend std::ostream & operator<<(std::ostream &os, const OPALSettings &sets);

public:
  /**
   * @brief Radius of initialization window.
   *
   * Window is square, each side (2 * initWindowRadius + 1) pixels.
   */
  size_t initWindowRadius;

  /// Side of inittialization window. initWindowSide = 2 * initWindowRadis + 1.
  size_t initWindowSide;

  /**
   * @brief Radius of patches OPAL operates with.
   *
   * Each patch is square with (2 * patchRadius + 1) pixels on side.
   */
  size_t patchRadius;

  /// Side of OPAL patches. patchSide = 2 * patchRadis + 1.
  size_t patchSide;

  /// Enable/disable intermediate saving.
  bool intermediateSaving;

  /// Path to save intermediate results.
  std::string intermediateSavingPath;

  /// Maximum number of iterations performed.
  size_t maxIterations;
};
