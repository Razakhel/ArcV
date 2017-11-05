#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif

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
  Matrix<> res(sobelMat.getWidth(),
               sobelMat.getHeight(),
               sobelMat.getChannelCount(),
               sobelMat.getImgBitDepth(),
               sobelMat.getColorspace());

  for (std::size_t i = 0; i < sobelMat.getData().size(); ++i)
    res[i] = 180.f + std::atan2(verticalGradient[i], horizontalGradient[i]) * (180.f / static_cast<float>(M_PI));

  return res;
}

Matrix<> Sobel::computeHorizontalSobelOperator(const Matrix<>& mat) {
  const Matrix<float> horizKernel = {{ 1.f, 0.f, -1.f },
                                     { 2.f, 0.f, -2.f },
                                     { 1.f, 0.f, -1.f }};

  return mat.convolve(horizKernel);
}

Matrix<> Sobel::computeVerticalSobelOperator(const Matrix<>& mat) {
  const Matrix<float> vertKernel = {{  1.f,  2.f,  1.f },
                                    {  0.f,  0.f,  0.f },
                                    { -1.f, -2.f, -1.f }};

  return mat.convolve(vertKernel);
}

} // namespace Arcv
