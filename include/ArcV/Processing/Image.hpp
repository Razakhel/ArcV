#pragma once

#ifndef ARCV_IMAGE_HPP
#define ARCV_IMAGE_HPP

#include <string>

#include "ArcV/Math/Matrix.hpp"

enum ImageType { ARCV_IMAGE_TYPE_JPEG = 0,
                 ARCV_IMAGE_TYPE_PNG };

enum FilterType { ARCV_FILTER_TYPE_GAUSSIAN_BLUR = 0,
                  ARCV_FILTER_TYPE_SHARPEN,
                  ARCV_FILTER_TYPE_EDGE_ENHANCEMENT,
                  ARCV_FILTER_TYPE_EMBOSS,
                  ARCV_FILTER_TYPE_SOBEL };

enum DetectorType { ARCV_DETECTOR_TYPE_CANNY = 0,
                    ARCV_DETECTOR_TYPE_HARRIS };

namespace Arcv {

const unsigned int PNG_HEADER_SIZE = 8;

class Image {
public:
  static Matrix<> read(const std::string& fileName);
  static void write(const Matrix<>& mat, const std::string& fileName);
  template <Colorspace C> static Matrix<> changeColorspace(Matrix<> mat);
  template <FilterType F> static Matrix<> applyFilter(Matrix<> mat);
  template <DetectorType D> static Matrix<> applyDetector(Matrix<> mat);
  template <typename T> static Matrix<T> threshold(Matrix<T> mat, T firstLowerBound, T firstUpperBound,
                                                                  T secondLowerBound, T secondUpperBound,
                                                                  T thirdLowerBound, T thirdUpperBound);
};

} // namespace Arcv

#include "ArcV/Processing/Image.inl"

#endif // ARCV_IMAGE_HPP
