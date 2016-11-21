#pragma once

#include <cstdio>
#include <cstring>
#include <iostream>


namespace Analyze {

struct HeaderKey {
  int32_t SizeOfHeader;
  char    DataType    [10];
  char    DatabaseName[18];
  int32_t Extents;
  int16_t SessionError;
  char    Regular;
  char    HKeyUn0;
} __attribute__((packed)); // 40 bytes.


struct ImageDimensions {
  int16_t Dimensions     [8];
  int16_t UnusedInt      [7];
  int16_t DataType;
  int16_t BitsPerPixel;
  int16_t DimUn0;
  float   PixelDimensions[8];
  float   VoxelOffset;
  float   UnusedFloat    [3];
  float   CalibrationMax;
  float   CalibrationMin;
  float   Compressed;
  float   Verified;
  int32_t GlobalMax;
  int32_t GlobalMin;
} __attribute__((packed)); // 108 bytes.


struct DataHistory {
  char    Description   [80];
  char    AuxFile       [24];
  char    Orientation;
  char    Originator    [10];
  char    Generated     [10];
  char    Scannum       [10];
  char    PatientId     [10];
  char    ExperimentDate[10];
  char    ExperimentTime[10];
  char    HistUn0       [3];
  int32_t Views;
  int32_t VolumesAdded;
  int32_t StartField;
  int32_t FieldSkip;
  int32_t OMax;
  int32_t OMin;
  int32_t SMax;
  int32_t SMin;
} __attribute__((packed)); // 200 bytes.


class AnalyzeFileHeader {
public:
  struct Data {
    HeaderKey       key;
    ImageDimensions dims;
    DataHistory     hist;
  } __attribute__((packed)); // 348 bytes.

  AnalyzeFileHeader();
  AnalyzeFileHeader(FILE *f);

  void ReadFromFile(FILE *f);
  Data GetData() const { return data; }

  friend std::ostream & operator<<(std::ostream &os,
                                   const AnalyzeFileHeader &hdr);

private:
  void PrintKey(std::ostream &os) const;
  void PrintImageDimensions(std::ostream &os) const;
  void PrintDataHistory(std::ostream &os) const;
  static const char* DecodeOrientation(char orient);


  Data data;
};

} // namespace Analyze
