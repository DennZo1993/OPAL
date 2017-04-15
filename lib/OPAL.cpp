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
      FieldT(i, j) = t;
      FieldX(i, j) = offsetX;
      FieldY(i, j) = offsetY;

    } // for (j)
  } // for (i)

  UpdateSSDMap();

  // Save current result.
  SaveCurrentFields("0_Initialization.flo");
}


void OPAL::Propagation(size_t iteration) {
  size_t xStart = iteration % 2 ? ImageWidth - Sets.patchRadius - 1
                                : Sets.patchRadius;

  size_t xEnd   = iteration % 2 ? Sets.patchRadius
                                : ImageWidth - Sets.patchRadius - 1;

  size_t yStart = iteration % 2 ? ImageHeight - Sets.patchRadius - 1
                                : Sets.patchRadius;

  size_t yEnd   = iteration % 2 ? Sets.patchRadius
                                : ImageHeight - Sets.patchRadius - 1;

  int    delta  = iteration % 2 ? -1 : 1;

  size_t propagatedPixels = 0;
  for (size_t y = yStart; y != yEnd; y += delta)
    for (size_t x = xStart; x != xEnd; x += delta) {
      propagatedPixels += PropagatePixel(y, x, delta);
    }

  //std::cout << propagatedPixels << std::endl;
  SaveCurrentFields("Iteration_" + std::to_string(iteration));
}

//#define DUMP

int OPAL::PropagatePixel(size_t i, size_t j, int delta) {
  SSDType current    = SSDMap(i, j);
  SSDType vertical   = SSDMap(i + delta, j);
  SSDType horizontal = SSDMap(i, j + delta);

  if (current < vertical && current < horizontal)
    return 0;

  SSDType fromVertical = vertical;
  SSDType fromHorizontal = horizontal;
  if (delta > 0) {
    fromHorizontal.ShiftLeft();
    fromVertical.ShiftUp();
  } else {
    fromVertical.ShiftDown();
    fromHorizontal.ShiftRight();
  }

  if (fromVertical < current && fromVertical < fromHorizontal) {
    FieldX(i, j) = FieldX(i + delta, j);
    FieldY(i, j) = FieldY(i + delta, j);
    FieldT(i, j) = FieldT(i + delta, j);
    SSDMap(i, j) = fromVertical;

    return 1;
  }

  if (fromHorizontal < current && fromHorizontal < fromVertical) {
    FieldX(i, j) = FieldX(i, j + delta);
    FieldY(i, j) = FieldY(i, j + delta);
    FieldT(i, j) = FieldT(i, j + delta);
    SSDMap(i, j) = fromHorizontal;

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
  for (size_t i = 0; i < Sets.maxIterations; ++i)
    Propagation(i);

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
  const auto &curDst = Database.GetImage(FieldT(i, j));
  auto curX = j + FieldX(i, j);
  auto curY = i + FieldY(i, j);

  return SSDType(&InputImage, i, j,
                 &curDst, curY, curX,
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
