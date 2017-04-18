#include "OPAL.h"
#include <stdexcept>
#include "../tools/FloFileIO.h"

static std::string
FullFileName(const std::string &path, const std::string &name) {
  std::string result = path;

  if (!result.empty() && result.back() != '/')
    result += "/";

  return result += name;
}


OPAL::OPAL(const OPALSettings &settings, const DatabaseType &database)
  : Sets(settings)
  , Database(database)
{
  if (Database.IsEmpty())
    throw std::logic_error("Image database is empty!");
  // The first pair in the DB is image to be segmented.
  // We need more images to proceed.
  if (Database.GetImageCount() == 1)
    throw std::logic_error("Image database contains only 1 pair!");

  ImageHeight = Database.GetImageHeight();
  ImageWidth = Database.GetImageWidth();

  InputImage = Database.GetImage(0);
  OutputSegmentation.Resize(ImageHeight, ImageWidth);

  // Allocate memory, but don't fill.
  FieldX.Resize(ImageHeight, ImageWidth);
  FieldY.Resize(ImageHeight, ImageWidth);
  FieldT.Resize(ImageHeight, ImageWidth);
  SSDMap.Resize(ImageHeight, ImageWidth);

  // Initialize random generator.
  std::random_device rd;
  randGen.seed(0);
}


void OPAL::ConstrainedInitialization() {
  int distLowerBound = (-1) * static_cast<int>(Sets.initWindowRadius);
  int distUpperBound = static_cast<int>(Sets.initWindowRadius);
  std::uniform_int_distribution<int> distT(1, Database.GetImageCount()-1);
  std::uniform_int_distribution<int> distX(distLowerBound, distUpperBound);

  // (i, j) is mapped to (i+offsetY, j+offsetX) at Database[t].
  // Make sure we'll be able to consider a patch around destination loc.
  int xLowerConstraint = Sets.patchRadius + 1;
  int yLowerConstraint = Sets.patchRadius + 1;
  int yUpperConstraint = ImageHeight - Sets.patchRadius - 2;
  int xUpperConstraint = ImageWidth - Sets.patchRadius - 2;

  // Fill FieldX, FieldY, FieldT.
  for (int i = 0; i < static_cast<int>(ImageHeight); ++i) {
    for (int j = 0; j < static_cast<int>(ImageWidth); ++j) {

      // Index of image in Database.
      size_t t = distT(randGen);
      // x and y coordinates.
      int offsetX = distX(randGen);
      int offsetY = distX(randGen); // can use distX for y (same bounds).

      // y coordinate
      if (i + offsetY < yLowerConstraint)
        offsetY = yLowerConstraint - i; // (i, j) -> (patchRadius, ...)
      if (i + offsetY > yUpperConstraint)
        offsetY = yUpperConstraint - i;

      // x coordinate
      if (j + offsetX < xLowerConstraint)
        offsetX = xLowerConstraint - j;
      if (j + offsetX > xUpperConstraint)
        offsetX = xUpperConstraint - j;

      // Initialize the fields.
      FieldT(i, j) = t;
      FieldX(i, j) = offsetX;
      FieldY(i, j) = offsetY;

    } // for (j)
  } // for (i)

  UpdateSSDMap();

  // Save current result.
  SaveCurrentFields("0_Initialization.flo");
}


void OPAL::EvenPropagation(size_t iteration)
{
  size_t propagatedPixels = 0;

  size_t xStart = Sets.patchRadius + 1;
  size_t xEnd   = ImageWidth - Sets.patchRadius - 2;
  size_t yStart = Sets.patchRadius + 1;
  size_t yEnd   = ImageHeight - Sets.patchRadius - 2;

  for (size_t y = yStart; y < yEnd; ++y)
    for (size_t x = xStart; x < xEnd; ++x) {
      propagatedPixels += PropagateRightDown(x, y);
    }

  SaveCurrentFields("Iteration_" + std::to_string(iteration));
}


void OPAL::OddPropagation(size_t iteration)
{
  size_t propagatedPixels = 0;

  size_t xStart = ImageWidth - Sets.patchRadius - 2;
  size_t xEnd   = Sets.patchRadius + 1;
  size_t yStart = ImageHeight - Sets.patchRadius - 2;
  size_t yEnd   = Sets.patchRadius + 1;

  for (size_t y = yStart; y > yEnd; --y)
    for (size_t x = xStart; x > xEnd; --x) {
      propagatedPixels += PropagateLeftUp(x, y);
    }

  SaveCurrentFields("Iteration_" + std::to_string(iteration));
}


int OPAL::PropagateRightDown(size_t x, size_t y)
{
  size_t newX = x + 1;
  size_t newY = y + 1;

  SSDType current   = SSDMap(y, x);
  SSDType fromDown  = SSDMap(newY, x);
  SSDType fromRight = SSDMap(y, newX);

  fromDown.ShiftUp();
  fromRight.ShiftLeft();

  if (current < fromRight && current < fromDown)
    return 0;

  if (fromDown < current && fromDown < fromRight) {
    FieldX(y, x) = FieldX(newY, x);
    FieldY(y, x) = FieldY(newY, x);
    FieldT(y, x) = FieldT(newY, x);
    SSDMap(y, x) = fromDown;

    return 1;
  }

  if (fromRight < current && fromRight < fromDown) {
    FieldX(y, x) = FieldX(y, newX);
    FieldY(y, x) = FieldY(y, newX);
    FieldT(y, x) = FieldT(y, newX);
    SSDMap(y, x) = fromRight;

    return 1;
  }

  return 0;
}


int OPAL::PropagateLeftUp(size_t x, size_t y)
{
  size_t newX = x - 1;
  size_t newY = y - 1;

  SSDType current  = SSDMap(y, x);
  SSDType fromUp   = SSDMap(newY, x);
  SSDType fromLeft = SSDMap(y, newX);

  fromUp.ShiftDown();
  fromLeft.ShiftRight();

  if (current < fromUp && current < fromLeft)
    return 0;

  if (fromUp < current && fromUp < fromLeft) {
    FieldX(y, x) = FieldX(newY, x);
    FieldY(y, x) = FieldY(newY, x);
    FieldT(y, x) = FieldT(newY, x);
    SSDMap(y, x) = fromUp;

    return 1;
  }

  if (fromLeft < current && fromLeft < fromUp) {
    FieldX(y, x) = FieldX(y, newX);
    FieldY(y, x) = FieldY(y, newX);
    FieldT(y, x) = FieldT(y, newX);
    SSDMap(y, x) = fromLeft;

    return 1;
  }

  return 0;
}


void OPAL::BuildSegmentation() {
  CandidateLabelsContainer candidates;

  size_t mismatchTimes = 0;

  for (size_t i = Sets.patchRadius; i + Sets.patchRadius < ImageHeight; ++i)
    for (size_t j = Sets.patchRadius; j + Sets.patchRadius < ImageWidth; ++j) {
      const auto &curDst = Database.GetSegmentation(FieldT(i, j));
      const auto OffsetX = FieldX(i, j);
      const auto OffsetY = FieldY(i, j);

      auto oldRes = curDst(i + OffsetY, j + OffsetX);
      GetCandidateLabelsForPixel(i, j, candidates);
      OutputSegmentation(i,j) = finalLabelEstimator.EstimateLabel(candidates);

      if (OutputSegmentation(i,j) != oldRes)
        ++mismatchTimes;
    }

  std::cout << "Mismatch times: " << mismatchTimes << std::endl;
}


void OPAL::Run() {
  ConstrainedInitialization();
  for (size_t i = 0; i < Sets.maxIterations; ++i) {
    if (i % 2 == 0)
      EvenPropagation(i);
    else
      OddPropagation(i);
  }

  BuildSegmentation();
}


void OPAL::SaveCurrentFields(const std::string &fileName) const {
  // If intermediate saving is not enabled, we are done.
  if (!Sets.intermediateSaving)
    return;

  auto fullFileName = FullFileName(Sets.intermediateSavingPath, fileName);
  FlowIO::WriteFlowFile(fullFileName, FieldX, FieldY);
}


OPAL::SSDType OPAL::SSDAt(size_t i, size_t j) const {
  auto movIndex = FieldT(i, j);
  auto movX = j + FieldX(i, j);
  auto movY = i + FieldY(i, j);

  return SSDType(Database, movIndex,
                 j, i, movX, movY,
                 Sets.patchRadius);
}


void OPAL::UpdateSSDMap() {
  for (size_t i = Sets.patchRadius; i + Sets.patchRadius < ImageHeight; ++i) {
    for (size_t j = Sets.patchRadius; j + Sets.patchRadius < ImageWidth; ++j) {
      SSDMap(i, j) = SSDAt(i, j);
    } // for (j)
  } // for (i)
}

void OPAL::GetCandidateLabelsForPixel(
    size_t i, size_t j, OPAL::CandidateLabelsContainer &result) const
{
  assert(i >= Sets.patchRadius && i + Sets.patchRadius < ImageHeight &&
         "index i is out of range!");
  assert(j >= Sets.patchRadius && j + Sets.patchRadius < ImageWidth &&
         "index j is out of range!");

  constexpr double CANDIDATE_WEIGHT = 1.0;

  result.clear();

  for (size_t di = i - Sets.patchRadius; di <= i + Sets.patchRadius; ++di)
    for (size_t dj = j - Sets.patchRadius; dj <= j + Sets.patchRadius; ++dj) {
      const auto &curDst = Database.GetSegmentation(FieldT(di, dj));
      const auto OffsetX = FieldX(di, dj);
      const auto OffsetY = FieldY(di, dj);

      const auto label = curDst(di + OffsetY, dj + OffsetX);

      result.push_back(std::make_pair(label, CANDIDATE_WEIGHT));
    }
}
