#pragma once

#ifndef ARCV_SOBEL_HPP
#define ARCV_SOBEL_HPP

#include "ArcV/Math/Matrix.hpp"

namespace Arcv {

namespace {

Matrix<> computeHorizontalSobelOperator(const Matrix<>& mat) {
  const Matrix<float> horizKernel = {{ 1.f, 0.f, -1.f },
                                     { 2.f, 0.f, -2.f },
                                     { 1.f, 0.f, -1.f }};

  return mat.convolve(horizKernel);
}

Matrix<> computeVerticalSobelOperator(const Matrix<>& mat) {
  const Matrix<float> vertKernel = {{  1.f,  2.f,  1.f },
                                    {  0.f,  0.f,  0.f },
                                    { -1.f, -2.f, -1.f }};

  return mat.convolve(vertKernel);
}

} // namespace

class Sobel {
public:
  Sobel(const Matrix<>& mat);

  const Matrix<>& getSobelMat() const { return sobelMat; }
  Matrix<>& getSobelMat() { return sobelMat; }
  const Matrix<>& getHorizontalGradient() const { return horizontalGradient; }
  Matrix<>& getHorizontalGradient() { return horizontalGradient; }
  const Matrix<>& getVerticalGradient() const { return verticalGradient; }
  Matrix<>& getVerticalGradient() { return verticalGradient; }

  Matrix<> computeGradientDirection() const;

private:
  Matrix<> sobelMat;
  Matrix<> horizontalGradient;
  Matrix<> verticalGradient;
};

} // namespace Arcv

#endif // ARCV_SOBEL_HPP
