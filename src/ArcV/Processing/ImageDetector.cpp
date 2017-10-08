#include "ArcV/Processing/Image.hpp"

namespace Arcv {

namespace Image {

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
