#include "OPAL.h"
#include <stdexcept>

OPAL::OPAL(const OPALSettings &settings, const DatabaseType &database)
  : Sets(settings)
  , Database(database)
{
  if (Database.isEmpty())
    throw std::logic_error("Image database is empty!");
  // The first pair in the DB is image to be segmented.
  // We need more images to proceed.
  if (Database.getImageCount() == 1)
    throw std::logic_error("Image database contains only 1 pair!");

  ImageHeight = Database.getImageHeight();
  ImageWidth = Database.getImageWidth();

  InputImage = Database.getImage(0);

  // Allocate memory, but don't fill.
  FieldX.Allocate(ImageHeight, ImageWidth);
  FieldY.Allocate(ImageHeight, ImageWidth);
  FieldT.Allocate(ImageHeight, ImageWidth);
  SSDMap.Allocate(ImageHeight, ImageWidth);

  // Initialize random generator.
  std::random_device rd;
  randGen.seed(rd());
}


void OPAL::ConstrainedInitialization() {
  int distLowerBound = (-1) * static_cast<int>(Sets.initWindowRadius);
  int distUpperBound = static_cast<int>(Sets.initWindowRadius);
  std::uniform_int_distribution<int> distT(1, Database.getImageCount()-1);
  std::uniform_int_distribution<int> distX(distLowerBound, distUpperBound);

  // (i, j) is mapped to (i+offsetY, j+offsetX) at Database[t].
  // Make sure we'll be able to consider a patch around destination loc.
  int yUpperConstraint = ImageHeight - Sets.patchRadius - 1;
  int xUpperConstraint = ImageWidth - Sets.patchRadius - 1;

  // Fill FieldX, FieldY, FieldT.
  for (int i = 0; i < static_cast<int>(ImageHeight); ++i) {
    for (int j = 0; j < static_cast<int>(ImageWidth); ++j) {

      // Index of image in Database.
      size_t t = distT(randGen);
      // x and y coordinates.
      int offsetX = distX(randGen);
      int offsetY = distX(randGen); // can use distX for y (same bounds).

      // y coordinate
      if (i + offsetY < static_cast<int>(Sets.patchRadius))
        offsetY = Sets.patchRadius - i; // (i, j) -> (patchRadius, ...)
      if (i + offsetY > yUpperConstraint)
        offsetY = yUpperConstraint - i;

      // x coordinate
      if (j + offsetX < static_cast<int>(Sets.patchRadius))
        offsetX = Sets.patchRadius - j;
      if (j + offsetX > xUpperConstraint)
        offsetX = xUpperConstraint - j;

      // Initialize the fields.
      FieldT[i][j] = t;
      FieldX[i][j] = offsetX;
      FieldY[i][j] = offsetY;

    } // for (j)
  } // for (i)

  UpdateSSDMap();
}


void OPAL::UpdateSSDMap() {
  for (size_t i = Sets.patchRadius; i + Sets.patchRadius < ImageHeight; ++i) {
    for (size_t j = Sets.patchRadius; j + Sets.patchRadius < ImageWidth; ++j) {
      const auto &curDst = Database.getImage(FieldT[i][j]);
      auto curX = j + FieldX[i][j];
      auto curY = i + FieldY[i][j];
      SSDMap[i][j] =
        InputImage.SSD(curDst,
                       i - Sets.patchRadius, j - Sets.patchRadius,
                       curY - Sets.patchRadius, curX - Sets.patchRadius,
                       Sets.patchSide, Sets.patchSide);
    } // for (j)
  } // for (i)
}
