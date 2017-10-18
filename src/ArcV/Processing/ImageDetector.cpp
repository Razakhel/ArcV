#include "ArcV/Processing/Image.hpp"
#include "ArcV/Processing/Sobel.hpp"

namespace Arcv {

namespace Image {

template <>
Matrix<> applyDetector<ARCV_DETECTOR_TYPE_CANNY>(Matrix<> mat) {
  const Sobel sobel(applyFilter<ARCV_FILTER_TYPE_GAUSSIAN_BLUR>(changeColorspace<ARCV_COLORSPACE_GRAY>(mat)));
  const Matrix<> directionMat = sobel.computeGradientDirection();
  Matrix<> res = sobel.getSobelMat();

  for (std::size_t heightIndex = 1; heightIndex < mat.getHeight() - 1; ++heightIndex) {
    for (std::size_t widthIndex = 1; widthIndex < mat.getHeight() - 1; ++widthIndex) {
      const std::size_t matIndex = heightIndex * mat.getWidth() + widthIndex;
      const std::size_t upPixIndex = (heightIndex - 1) * mat.getWidth() + widthIndex;
      const std::size_t lowPixIndex = (heightIndex + 1) * mat.getWidth() + widthIndex;
      const std::size_t rightPixIndex = heightIndex * mat.getWidth() + widthIndex + 1;
      const std::size_t leftPixIndex = heightIndex * mat.getWidth() + widthIndex - 1;
      const std::size_t upperRightPixIndex = (heightIndex - 1) * mat.getWidth() + widthIndex + 1;
      const std::size_t upperLeftPixIndex = (heightIndex - 1) * mat.getWidth() + widthIndex - 1;
      const std::size_t lowerRightPixIndex = (heightIndex + 1) * mat.getWidth() + widthIndex + 1;
      const std::size_t lowerLeftPixIndex = (heightIndex + 1) * mat.getWidth() + widthIndex - 1;

      if ((directionMat[matIndex] >= 0.f && directionMat[matIndex] <= 22.5f)
          || (directionMat[matIndex] >= 157.5f && directionMat[matIndex] <= 202.5f)
          || (directionMat[matIndex] >= 337.5f && directionMat[matIndex] <= 360.f)) {
        if (sobel.getSobelMat()[rightPixIndex] > sobel.getSobelMat()[matIndex]
            || sobel.getSobelMat()[leftPixIndex] > sobel.getSobelMat()[matIndex])
          res[matIndex] = 0.f;
      } else if ((directionMat[matIndex] > 22.5f && directionMat[matIndex] <= 67.5f)
                 || (directionMat[matIndex] > 202.5f && directionMat[matIndex] <= 247.5f)) {
        if (sobel.getSobelMat()[lowerRightPixIndex] > sobel.getSobelMat()[matIndex]
            || sobel.getSobelMat()[upperLeftPixIndex] > sobel.getSobelMat()[matIndex])
          res[matIndex] = 0.f;
      } else if ((directionMat[matIndex] > 67.5f && directionMat[matIndex] <= 112.5f)
                 || (directionMat[matIndex] > 247.5f && directionMat[matIndex] <= 292.5f)) {
        if (sobel.getSobelMat()[upPixIndex] > sobel.getSobelMat()[matIndex]
            || sobel.getSobelMat()[lowPixIndex] > sobel.getSobelMat()[matIndex])
          res[matIndex] = 0.f;
      } else if ((directionMat[matIndex] > 112.5f && directionMat[matIndex] <= 157.5f)
                 || (directionMat[matIndex] > 292.5f && directionMat[matIndex] <= 337.5f)) {
        if (sobel.getSobelMat()[upperRightPixIndex] > sobel.getSobelMat()[matIndex]
            || sobel.getSobelMat()[lowerLeftPixIndex] > sobel.getSobelMat()[matIndex])
          res[matIndex] = 0.f;
      }
    }
  }

  return threshold<ARCV_THRESH_TYPE_HYSTERESIS>(res, { 100.f }, { 150.f });
}

template <>
Matrix<> applyDetector<ARCV_DETECTOR_TYPE_HARRIS>(Matrix<> mat) {
  mat = changeColorspace<ARCV_COLORSPACE_GRAY>(mat);

  Matrix<float> horizRes = computeHorizontalSobelOperator(mat);
  Matrix<float> vertRes = computeVerticalSobelOperator(mat);

  Matrix<float> res = horizRes * vertRes;
  horizRes *= horizRes;
  vertRes *= vertRes;

  for (std::size_t i = 0; i < mat.getData().size(); ++i) {
    if ((horizRes.getData()[i] * vertRes.getData()[i] - res.getData()[i] * res.getData()[i])
        - 0.04f * std::pow(horizRes.getData()[i] + vertRes.getData()[i], 2.f) > 255)
      mat.getData()[i] = 255;
  }

  return mat;
}

} // namespace Image

} // namespace Arcv
