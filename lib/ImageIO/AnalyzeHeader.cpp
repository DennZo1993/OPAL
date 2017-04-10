#include "AnalyzeHeader.h"

#include <stdexcept>


template <class ToCheck,
          std::size_t ExpectedSize, std::size_t RealSize = sizeof(ToCheck)>
static void CheckSize() {
  static_assert(ExpectedSize == RealSize, "CheckSize failed: size mismatch!");
}


namespace Analyze {

AnalyzeFileHeader::AnalyzeFileHeader() {
  CheckSize<HeaderKey, 40>();
  CheckSize<ImageDimensions, 108>();
  CheckSize<DataHistory, 200>();
  CheckSize<Data, 348>();

  std::memset(&data, 0, sizeof(Data));
}


AnalyzeFileHeader::AnalyzeFileHeader(FILE *f) {
  std::memset(&data, 0, sizeof(Data));
  ReadFromFile(f);
}


void AnalyzeFileHeader::ReadFromFile(FILE *f) {
  if (!f)
    throw std::runtime_error("Failed to read Analyze header: FILE* is null");

  auto elems = fread(&data, sizeof(Data), 1, f);
  if (elems != 1)
    throw std::runtime_error("Failed to read Analyze header");
}


std::ostream & operator<<(std::ostream &os,
                          const AnalyzeFileHeader &hdr) {
  hdr.PrintKey(os);
  hdr.PrintImageDimensions(os);
  hdr.PrintDataHistory(os);
  return os;
}


void AnalyzeFileHeader::PrintKey(std::ostream &os) const {
  os << "Header key {"
     << "\n  SizeOfHeader   : "  << data.key.SizeOfHeader
     << "\n  DataType       : '" << data.key.DataType << "'"
     << "\n  DatabaseName   : '" << data.key.DatabaseName << "'"
     << "\n  Extents        : "  << data.key.Extents
     << "\n  SessionError   : "  << data.key.SessionError
     << "\n  Regular        : '" << data.key.Regular << "'"
     << "\n  HKeyUn0        : '" << data.key.HKeyUn0 << "'"
     << "\n} (" << sizeof(data.key) << " bytes)\n";
}


void AnalyzeFileHeader::PrintImageDimensions(std::ostream &os) const {
  os << "Image dimensions {"
     << "\n  Dimensions {"
     << "\n    Total number   : " << data.dims.Dimensions[0]
     << "\n    X dimension    : " << data.dims.Dimensions[1]
     << "\n    Y dimension    : " << data.dims.Dimensions[2]
     << "\n    Z dimension    : " << data.dims.Dimensions[3]
     << "\n    Volumes        : " << data.dims.Dimensions[4]
     << "\n  }                (6  bytes unused)"
     << "\n  UnusedInt      (14 bytes unused)"
     << "\n  DataType       : " << data.dims.DataType
     << "\n  BitsPerPixel   : " << data.dims.BitsPerPixel
     << "\n  DimUn0         : " << data.dims.DimUn0
     << "\n  PixelDimensions {"
     << "\n    Unused         : " << data.dims.PixelDimensions[0]
     << "\n    Voxel width  mm: " << data.dims.PixelDimensions[1]
     << "\n    Voxel height mm: " << data.dims.PixelDimensions[2]
     << "\n    Slice thickness: " << data.dims.PixelDimensions[0]
     << "\n  }                (8  bytes unused)"
     << "\n  VoxelOffset    : " << data.dims.VoxelOffset
     << "\n  UnusedFloat    (12 bytes unused)"
     << "\n  CalibrationMax : " << data.dims.CalibrationMax
     << "\n  CalibrationMin : " << data.dims.CalibrationMin
     << "\n  Verified       : " << data.dims.Verified
     << "\n  GlobalMax      : " << data.dims.GlobalMax
     << "\n  GlobalMin      : " << data.dims.GlobalMin
     << "\n} (" << sizeof(data.dims) << " bytes)\n";
}

void AnalyzeFileHeader::PrintDataHistory(std::ostream &os) const {
  os << "DataHistory {"
     << "\n  Orientation    : " << DecodeOrientation(data.hist.Orientation)
     << "\n} (" << sizeof(data.hist) << " bytes)\n";
}

const char* AnalyzeFileHeader::DecodeOrientation(char orient) {
  switch (orient) {
  case 0 : return "transverse unflipped";
  case 1 : return "coronal unflipped";
  case 2 : return "sagittal unflipped";
  case 3 : return "transverse flipped";
  case 4 : return "coronal flipped";
  case 5 : return "sagittal flipped";
  default: return "unknown";
  }
}


} // namespace Analyze
