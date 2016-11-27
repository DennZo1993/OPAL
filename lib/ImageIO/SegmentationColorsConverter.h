#pragma once

#include "../RGBAPixel.h"
#include <map>
#include <stdexcept>


namespace ImageIO {

class SegmentationColorsConverter {
public:
  SegmentationColorsConverter() {
    ColorsMap[0] = RGBAPixel(0, 0, 0);
    ColorsMap[1] = RGBAPixel(0, 0, 0);
    ColorsMap[2] = RGBAPixel(0, 238, 0);
    ColorsMap[3] = RGBAPixel(160, 82, 45);
    ColorsMap[4] = RGBAPixel(255, 218, 185);
    ColorsMap[5] = RGBAPixel(0, 206, 209);
    ColorsMap[6] = RGBAPixel(127, 255, 212);
    ColorsMap[7] = RGBAPixel(178, 34, 34);
    ColorsMap[8] = RGBAPixel(238, 0, 0);
    ColorsMap[9] = RGBAPixel(34, 139, 34);
    ColorsMap[10] = RGBAPixel(208, 32, 144);
    ColorsMap[11] = RGBAPixel(173, 255, 47);
    ColorsMap[12] = RGBAPixel(240, 230, 140);
    ColorsMap[13] = RGBAPixel(173, 216, 230);
    ColorsMap[14] = RGBAPixel(238, 238, 0);
    ColorsMap[15] = RGBAPixel(50, 205, 50);
    ColorsMap[16] = RGBAPixel(255, 0, 255);
    ColorsMap[17] = RGBAPixel(176, 48, 96);
    ColorsMap[18] = RGBAPixel(0, 255, 127);
    ColorsMap[19] = RGBAPixel(245, 222, 179);
    ColorsMap[20] = RGBAPixel(255, 165, 5);
    ColorsMap[21] = RGBAPixel(255, 165, 0);
    ColorsMap[22] = RGBAPixel(255, 69, 0);
    ColorsMap[23] = RGBAPixel(205, 91, 69);
    ColorsMap[24] = RGBAPixel(255, 192, 203);
    ColorsMap[25] = RGBAPixel(152, 251, 152);
    ColorsMap[26] = RGBAPixel(100, 149, 237);
    ColorsMap[27] = RGBAPixel(160, 32, 240);
    ColorsMap[28] = RGBAPixel(238, 130, 238);
    ColorsMap[29] = RGBAPixel(238, 201, 0);
    ColorsMap[30] = RGBAPixel(218, 112, 214);
    ColorsMap[31] = RGBAPixel(255, 62, 150);
    ColorsMap[32] = RGBAPixel(0, 0, 255);
    ColorsMap[33] = RGBAPixel(39, 64, 139);
    ColorsMap[34] = RGBAPixel(250, 128, 114);
    ColorsMap[35] = RGBAPixel(255, 110, 180);
    ColorsMap[36] = RGBAPixel(255, 99, 71);
    ColorsMap[37] = RGBAPixel(255, 255, 0);
    ColorsMap[38] = RGBAPixel(0, 100, 0);
    ColorsMap[39] = RGBAPixel(205, 92, 92);
    ColorsMap[40] = RGBAPixel(165, 42, 42);
    ColorsMap[41] = RGBAPixel(153, 50, 204);
    ColorsMap[42] = RGBAPixel(0, 255, 255);
    ColorsMap[43] = RGBAPixel(221, 160, 221);
    ColorsMap[44] = RGBAPixel(135, 206, 235);
    ColorsMap[45] = RGBAPixel(152, 200, 214);
    ColorsMap[46] = RGBAPixel(153, 29, 242);
    ColorsMap[47] = RGBAPixel(210, 180, 140);
    ColorsMap[48] = RGBAPixel(255, 215, 0);
    ColorsMap[49] = RGBAPixel(0, 0, 128);
    ColorsMap[50] = RGBAPixel(46, 139, 87);
    ColorsMap[51] = RGBAPixel(102, 205, 170);
    ColorsMap[52] = RGBAPixel(0, 255, 0);
    ColorsMap[53] = RGBAPixel(0, 0, 0);
    ColorsMap[54] = RGBAPixel(220, 216, 20);
    ColorsMap[55] = RGBAPixel(60, 58, 210);
    ColorsMap[56] = RGBAPixel(100, 50, 100);
    ColorsMap[57] = RGBAPixel(135, 50, 74);
    ColorsMap[58] = RGBAPixel(122, 135, 50);
    ColorsMap[59] = RGBAPixel(51, 50, 135);
    ColorsMap[60] = RGBAPixel(74, 155, 60);
    ColorsMap[61] = RGBAPixel(0, 238, 0);
    ColorsMap[62] = RGBAPixel(160, 82, 45);
    ColorsMap[63] = RGBAPixel(255, 218, 185);
    ColorsMap[64] = RGBAPixel(0, 206, 209);
    ColorsMap[65] = RGBAPixel(127, 255, 212);
    ColorsMap[66] = RGBAPixel(178, 34, 34);
    ColorsMap[67] = RGBAPixel(238, 0, 0);
    ColorsMap[68] = RGBAPixel(34, 139, 34);
    ColorsMap[69] = RGBAPixel(208, 32, 144);
    ColorsMap[70] = RGBAPixel(173, 255, 47);
    ColorsMap[71] = RGBAPixel(240, 230, 140);
    ColorsMap[72] = RGBAPixel(173, 216, 230);
    ColorsMap[73] = RGBAPixel(238, 238, 0);
    ColorsMap[74] = RGBAPixel(50, 205, 50);
    ColorsMap[75] = RGBAPixel(255, 0, 255);
    ColorsMap[76] = RGBAPixel(176, 48, 96);
    ColorsMap[77] = RGBAPixel(0, 255, 127);
    ColorsMap[78] = RGBAPixel(245, 222, 179);
    ColorsMap[79] = RGBAPixel(255, 165, 5);
    ColorsMap[80] = RGBAPixel(255, 165, 0);
    ColorsMap[81] = RGBAPixel(255, 69, 0);
    ColorsMap[82] = RGBAPixel(205, 91, 69);
    ColorsMap[83] = RGBAPixel(255, 192, 203);
    ColorsMap[84] = RGBAPixel(152, 251, 152);
    ColorsMap[85] = RGBAPixel(100, 149, 237);
    ColorsMap[86] = RGBAPixel(160, 32, 240);
    ColorsMap[87] = RGBAPixel(238, 130, 238);
    ColorsMap[88] = RGBAPixel(238, 201, 0);
    ColorsMap[89] = RGBAPixel(218, 112, 214);
    ColorsMap[90] = RGBAPixel(255, 62, 150);
    ColorsMap[91] = RGBAPixel(0, 0, 255);
    ColorsMap[92] = RGBAPixel(39, 64, 139);
    ColorsMap[93] = RGBAPixel(250, 128, 114);
    ColorsMap[94] = RGBAPixel(255, 110, 180);
    ColorsMap[95] = RGBAPixel(255, 99, 71);
    ColorsMap[96] = RGBAPixel(255, 255, 0);
    ColorsMap[97] = RGBAPixel(0, 100, 0);
    ColorsMap[98] = RGBAPixel(205, 92, 92);
    ColorsMap[99] = RGBAPixel(165, 42, 42);
    ColorsMap[100] = RGBAPixel(153, 50, 204);
    ColorsMap[101] = RGBAPixel(0, 255, 255);
    ColorsMap[102] = RGBAPixel(221, 160, 221);
    ColorsMap[103] = RGBAPixel(135, 206, 235);
    ColorsMap[104] = RGBAPixel(152, 200, 214);
    ColorsMap[105] = RGBAPixel(153, 29, 242);
    ColorsMap[106] = RGBAPixel(210, 180, 140);
    ColorsMap[107] = RGBAPixel(255, 215, 0);
    ColorsMap[108] = RGBAPixel(0, 0, 128);
    ColorsMap[109] = RGBAPixel(46, 139, 87);
    ColorsMap[110] = RGBAPixel(102, 205, 170);
    ColorsMap[111] = RGBAPixel(0, 255, 0);
    ColorsMap[112] = RGBAPixel(0, 0, 0);
    ColorsMap[113] = RGBAPixel(220, 216, 20);
    ColorsMap[114] = RGBAPixel(60, 58, 210);
    ColorsMap[115] = RGBAPixel(100, 50, 100);
    ColorsMap[116] = RGBAPixel(135, 50, 74);
    ColorsMap[117] = RGBAPixel(122, 135, 50);
    ColorsMap[118] = RGBAPixel(51, 50, 135);
    ColorsMap[119] = RGBAPixel(74, 155, 60);
    ColorsMap[120] = RGBAPixel(1, 1, 1);
  }

  template <class SegmentationPixelType>
  Image<RGBAPixel> ConvertToRGB(const Image<SegmentationPixelType> &image) {
    Image<RGBAPixel> result(image.getHeight(), image.getWidth());
    for (size_t i = 0; i < image.getSize(); ++i) {
      result[i] = ColorsMap.at(image[i]);
    }
    return result;
  }

  template <class ImagePixelType, class SegmentationPixelType>
  Image<RGBAPixel> ConvertToRGB(const Image<ImagePixelType> &image,
                                const Image<SegmentationPixelType> &seg,
                                bool contoured = true) {
    if (image.getWidth() != seg.getWidth() ||
        image.getHeight() != seg.getHeight())
      throw std::invalid_argument("Image and segmentation size mismatch!");

    Image<RGBAPixel> result(image.getHeight(), image.getWidth());
    for (size_t i = 1; i + 1 < image.getHeight(); ++i) {
      for (size_t j = 1; j + 1 < image.getWidth(); ++j) {
        if (!seg(i, j)) {
          // No label here, just draw image pixel.
          result(i, j) = RGBAPixel(image(i, j));
        } else {
          // Segmentation label here.
          if (!contoured) {
            // Not contoured mode, just draw label.
            result(i, j) = ColorsMap.at(seg(i, j));
          } else {
            auto currentLabel = seg(i, j);
            bool insideStructure =
              seg(i-1, j) == currentLabel &&
              seg(i+1, j) == currentLabel &&
              seg(i, j-1) == currentLabel &&
              seg(i, j+1) == currentLabel;

            result(i, j) = insideStructure ? RGBAPixel(image(i, j))
                                            : ColorsMap.at(seg(i, j));
          }
        }
      } // for (j)
    } // for (i)
    return result;
  }

private:
  std::map<int, RGBAPixel> ColorsMap;
};

} // namespace ImageIO
