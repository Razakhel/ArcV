#pragma once

#ifndef ARCV_IMAGE_HPP
#define ARCV_IMAGE_HPP

#include <string>

#include "ArcV/Math/Matrix.hpp"

enum ImageType { ARCV_IMAGE_TYPE_JPEG = 0,
                 ARCV_IMAGE_TYPE_PNG };

enum FilterType { ARCV_FILTER_TYPE_GAUSSIAN_BLUR = 0 };

namespace Arcv {

const unsigned int PNG_HEADER_SIZE = 8;

class Image {
public:
  static Mat read(const std::string& fileName);
  static void write(const Mat& mat, const std::string& fileName);
  static const uint8_t getChannelCount(const Mat& mat);
  template <Colorspace C> static void changeColorspace(Mat& mat);
  template <FilterType F> static void applyFilter(Mat& mat);
};

} // namespace Arcv

#include "ArcV/Processing/Image.inl"

#endif // ARCV_IMAGE_HPP
