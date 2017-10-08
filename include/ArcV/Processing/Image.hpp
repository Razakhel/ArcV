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

enum ThreshType { ARCV_THRESH_TYPE_BINARY = 0,
                  ARCV_THRESH_TYPE_HYSTERESIS };

namespace Arcv {

namespace Image {

namespace {

Matrix<> computeHorizontalSobelOperator(Matrix<>& mat) {
  const Matrix<float> horizKernel = {{  1.f,  2.f,  1.f },
                                     {  0.f,  0.f,  0.f },
                                     { -1.f, -2.f, -1.f }};

  return mat.convolve(horizKernel);
}

Matrix<> computeVerticalSobelOperator(Matrix<>& mat) {
  const Matrix<float> vertKernel = {{ 1.f, 0.f, -1.f },
                                    { 2.f, 0.f, -2.f },
                                    { 1.f, 0.f, -1.f }};

  return mat.convolve(vertKernel);
}

} // namespace

Matrix<> read(const std::string& fileName);
void write(const Matrix<>& mat, const std::string& fileName);
template <Colorspace C> Matrix<> changeColorspace(Matrix<> mat);
template <FilterType F> Matrix<> applyFilter(Matrix<> mat);
template <DetectorType D> Matrix<> applyDetector(Matrix<> mat);
template <ThreshType Thresh> Matrix<> threshold(const Matrix<>& mat, std::initializer_list<float> lowerBounds,
                                                                     std::initializer_list<float> upperBounds);
template <typename T> Matrix<T> rotateLeft(const Matrix<T>& mat);
template <typename T> Matrix<T> rotateRight(const Matrix<T>& mat);
template <typename T> Matrix<T> reverse(const Matrix<T>& mat);
template <typename T> Matrix<T> horizontalFlip(const Matrix<T>& mat);
template <typename T> Matrix<T> verticalFlip(const Matrix<T>& mat);
template <typename T> Matrix<T> region(const Matrix<T>& mat, std::size_t widthBegin, std::size_t widthEnd,
                                                             std::size_t heightBegin, std::size_t heightEnd);

} // namespace Image

} // namespace Arcv

#include "ArcV/Processing/Image.inl"

#endif // ARCV_IMAGE_HPP
