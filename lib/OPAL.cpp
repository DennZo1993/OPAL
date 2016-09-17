#include "OPAL.h"

OPAL::OPAL(const DatabaseType &database) : Database(database) {
  if (Database.isEmpty())
    throw std::logic_error("Image database is empty!");
  // The first pair in the DB is image to be segmented.
  // We need more images to proceed.
  if (Database.getImageCount() == 1)
    throw std::logic_error("Image database contains only 1 pair!");

  ImageHeight = Database.getImageHeight();
  ImageWidth = Database.getImageWidth();

  // Allocate memory, but don't fill.
  FieldX.Allocate(ImageHeight, ImageWidth);
  FieldY.Allocate(ImageHeight, ImageWidth);
  FieldT.Allocate(ImageHeight, ImageWidth);
  SSDMap.Allocate(ImageHeight, ImageWidth);
}
