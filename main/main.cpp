#include "OPAL.h"
#include "../tools/FloFileIO.h"

int main(int argc, char **argv) {
  if (argc < 5) {
    std::cerr << "4 arguments excepted!" << std::endl;
    return 1;
  }

  try {
    ImageDatabase<double, int> db;
    db.Add(argv[1], argv[2]);
    db.Add(argv[3], argv[4]);

    OPALSettings settings = OPALSettings::GetDefaults();
    settings.intermediateSaving = true;
    OPAL opal(settings, db);
    opal.ConstrainedInitialization();
  } catch (std::runtime_error &ex) {
    std::cerr << "Runtime exception caught!\n" << ex.what() << std::endl;
    return 1;
  } catch (...) {
    std::cerr << "Unknown exception caught!" << std::endl;
    return 1;
  }

  return 0;
}
