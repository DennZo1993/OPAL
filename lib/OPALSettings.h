#pragma once

#include <string>

// A lightweight class containing various OPAL parameters.
struct OPALSettings {
public:
  OPALSettings(size_t _initWindowRadius
             , size_t _patchRadius
             , bool _intermediateSaving
             , const std::string &_savingPath
             , size_t _maxIter
              )
    : initWindowRadius(_initWindowRadius)
    , initWindowSide(2 * _initWindowRadius + 1)
    , patchRadius(_patchRadius)
    , patchSide(2 * _patchRadius + 1)
    , intermediateSaving(_intermediateSaving)
    , intermediateSavingPath(_savingPath)
    , maxIterations(_maxIter)
  {}


  // Radius of initialization window. Window is square,
  // each side (2 * initWindowRadius + 1) pixels.
  size_t initWindowRadius;
  size_t initWindowSide;

  // Radius of patches OPAL operates with. Each patch is
  // square with (2 * patchRadius + 1) pixels on side.
  size_t patchRadius;
  size_t patchSide;

  // Enable/disable intermediate saving.
  bool intermediateSaving;
  std::string intermediateSavingPath;

  size_t maxIterations;

  // Default values for all settings.
  static OPALSettings GetDefaults() {
    return OPALSettings(10    /* initWindowRadius       */
                      , 3     /* patchRadius            */
                      , false /* intermediateSaving     */
                      , ""    /* intermediateSavingPath */
                      , 30
                       );
  }
};
