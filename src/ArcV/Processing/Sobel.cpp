#include "ArcV/Processing/Sobel.hpp"

namespace Arcv {

Sobel::Sobel(const Matrix<>& mat) {
  sobelMat = Matrix<>(mat.getWidth(), mat.getHeight(), mat.getChannelCount(), mat.getImgBitDepth(), mat.getColorspace());
  horizontalGradient = computeHorizontalSobelOperator(mat);
  verticalGradient = computeVerticalSobelOperator(mat);

  for (std::size_t i = 0; i < mat.getData().size(); ++i)
    sobelMat.getData()[i] = std::sqrt(horizontalGradient.getData()[i] * horizontalGradient.getData()[i]
                                      + verticalGradient.getData()[i] * verticalGradient.getData()[i]);
}

Matrix<> Sobel::computeGradientDirection() const {
  Matrix<> res(sobelMat.getWidth(), sobelMat.getHeight(), sobelMat.getChannelCount(), sobelMat.getImgBitDepth(), sobelMat.getColorspace());

  for (std::size_t i = 0; i < sobelMat.getData().size(); ++i)
    res[i] = 180.f + std::atan2(verticalGradient[i], horizontalGradient[i]) * (180.f / M_PI);

  return res;
}

} // namespace Arcv
