#include "OPAL.h"
#include "ImageIO.h"
#include "SegmentationColorsConverter.h"
#include <iostream>

int main(int argc, char **argv) {
  if (argc < 6)
    return 0;

  OPAL::DatabaseType db;
  db.Add(argv[1], argv[2]);
  db.Add(argv[3], argv[4]);
  db.Add(argv[5], argv[6]);

  OPAL opal(OPALSettings::GetDefaults(), db);
  opal.Run();

  auto seg = opal.GetOutput();

  ImageIO::SegmentationColorsConverter<OPAL::SegmentationPixelType> converter;

  auto rgb = converter.ConvertToRGB(seg);

  ImageIO::WriteImage(rgb, argv[7]);

  return 0;
}
