#include "OPALSettings.h"
#include "util/string/Split.h"

#include <iostream>
#include <fstream>


OPALSettings::OPALSettings(size_t _initWindowRadius,
                           size_t _patchRadius,
                           bool _intermediateSaving,
                           const std::string &_savingPath,
                           size_t _maxIter)
  : initWindowRadius(_initWindowRadius)
  , initWindowSide(2 * _initWindowRadius + 1)
  , patchRadius(_patchRadius)
  , patchSide(2 * _patchRadius + 1)
  , intermediateSaving(_intermediateSaving)
  , intermediateSavingPath(_savingPath)
  , maxIterations(_maxIter)
{
}


OPALSettings::OPALSettings(const std::map<std::string, std::string> &sets)
  : initWindowRadius(std::stoul(sets.at("initWindowRadius")))
  , patchRadius(std::stoul(sets.at("patchRadius")))
  , intermediateSaving(sets.at("intermediateSaving") == "true")
  , intermediateSavingPath(sets.at("intermediateSavingPath"))
  , maxIterations(std::stoul(sets.at("maxIterations")))
{
  initWindowSide = 2 * initWindowRadius + 1;
  patchSide = 2 * patchRadius + 1;
}


std::map<std::string, std::string> OPALSettings::GetDefaultSetsMap()
{
  return {
    { "initWindowRadius",       "10"    },
    { "patchRadius",            "3"     },
    { "intermediateSaving",     "false" },
    { "intermediateSavingPath", ""      },
    { "maxIterations",          "30"    }
  };
}


OPALSettings OPALSettings::GetDefaults()
{
  return OPALSettings(GetDefaultSetsMap());
}


OPALSettings OPALSettings::ReadFromFile(const std::string &fileName)
{
  std::map<std::string, std::string> settings = GetDefaultSetsMap();

  std::ifstream ifs(fileName);
  std::string line;

  while (ifs >> line) {
    auto keyValue = util::SplitStringByLast(line, '=');
    settings[keyValue.first] = keyValue.second;
  }

  return OPALSettings(settings);
}


std::ostream & operator<<(std::ostream &os, const OPALSettings &sets)
{
  os << "OPAL settings:"
     << '\n' << "initWindowRadius       = " << sets.initWindowRadius
     << '\n' << "patchRadius            = " << sets.patchRadius
     << '\n' << "intermediateSaving     = " << sets.intermediateSaving
     << '\n' << "intermediateSavingPath = " << sets.intermediateSavingPath
     << '\n' << "maxIterations          = " << sets.maxIterations
     << std::endl;
  return os;
}
