#pragma once

#include "Image.h"
#include "RGBAPixel.h"

// Colorful image.
using RGBImage       = Image<RGBAPixel>;

// Image in shades of gray.
using GrayScaleImage = Image<unsigned char>;

// Black-and-white image.
using BinaryImage    = Image<bool>;

// Special representation of an image. In an integral image pixel at location
// (i,j) contains the sum of all pixels of source (gray scale!) image from
// (0,0) to (i,j).
using IntegralImage  = Image<unsigned long long>;
