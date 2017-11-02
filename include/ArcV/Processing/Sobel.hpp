#pragma once

#ifndef ARCV_SOBEL_HPP
#define ARCV_SOBEL_HPP

#include "ArcV/Math/Matrix.hpp"

namespace Arcv {

class Sobel {
public:
  Sobel(const Matrix<>& mat);

  const Matrix<>& getSobelMat() const { return sobelMat; }
  Matrix<>& getSobelMat() { return sobelMat; }
  const Matrix<>& getHorizontalGradient() const { return horizontalGradient; }
  Matrix<>& getHorizontalGradient() { return horizontalGradient; }
  const Matrix<>& getVerticalGradient() const { return verticalGradient; }
  Matrix<>& getVerticalGradient() { return verticalGradient; }

  static Matrix<> computeHorizontalSobelOperator(const Matrix<>& mat);
  static Matrix<> computeVerticalSobelOperator(const Matrix<>& mat);
  Matrix<> computeGradientDirection() const;

private:
  Matrix<> sobelMat;
  Matrix<> horizontalGradient;
  Matrix<> verticalGradient;
};

} // namespace Arcv

#endif // ARCV_SOBEL_HPP
