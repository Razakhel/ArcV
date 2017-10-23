#include "ArcV/Processing/Image.hpp"

namespace Arcv {

namespace Image {

template <>
Matrix<> threshold<ARCV_THRESH_TYPE_BINARY>(const Matrix<>& mat, std::initializer_list<float> lowerBounds,
                                                                 std::initializer_list<float> upperBounds) {
  assert(("Error: The number of lower & upper boundaries must match each other", lowerBounds.size() == upperBounds.size()));
  assert(("Error: The number of boundaries must match channel count",
          lowerBounds.size() + upperBounds.size() == mat.getChannelCount() * 2));

  Matrix<> res(mat.getWidth(), mat.getHeight(), 1, mat.getImgBitDepth(), ARCV_COLORSPACE_GRAY);
  std::size_t resIndex = 0;

  for (auto elt = mat.getData().begin(); elt != mat.getData().end(); elt += mat.getChannelCount(), ++resIndex) {
    bool inBounds = true;

    for (uint8_t chan = 0; chan < mat.getChannelCount(); ++chan) {
      const float val = *(elt + chan);

      if (val < *(lowerBounds.begin() + chan) || val > *(upperBounds.begin() + chan))
        inBounds = false;
    }

    res[resIndex] = (inBounds ? 255 : 0);
  }

  return res;
}

template <>
Matrix<> threshold<ARCV_THRESH_TYPE_HYSTERESIS>(const Matrix<>& mat, std::initializer_list<float> lowerBounds,
                                                                     std::initializer_list<float> upperBounds) {
  assert(("Error: There should be only one lower & one upper boundaries", lowerBounds.size() == 1 && upperBounds.size() == 1));

  Matrix<float> res = changeColorspace<ARCV_COLORSPACE_GRAY>(mat);
  const Matrix<float> temp = res;

  for (unsigned int heightIndex = 0; heightIndex < temp.getHeight(); ++heightIndex) {
    for (unsigned int widthIndex = 0; widthIndex < temp.getWidth(); ++widthIndex) {
      const std::size_t tempIndex = heightIndex * temp.getWidth() + widthIndex;
      float resVal = 0.f;

      if (temp[tempIndex] >= *upperBounds.begin()) {
        resVal = 255;
      } else {
        if (temp[tempIndex] >= *lowerBounds.begin()) {
          for (int8_t roundHeightIndex = -1; roundHeightIndex <= 1; ++roundHeightIndex) {
            for (int8_t roundWidthIndex = -1; roundWidthIndex <= 1; ++roundWidthIndex) {
              const std::size_t correspHeightIndex = heightIndex + roundHeightIndex;
              const std::size_t correspWidthIndex = widthIndex + roundWidthIndex;

              if ((correspHeightIndex < res.getHeight() && correspWidthIndex < res.getWidth())
                  && temp[correspHeightIndex * temp.getWidth() + correspWidthIndex] >= *upperBounds.begin()) {
                resVal = 255;
                break;
              }
            }
          }
        }
      }

      res[tempIndex] = resVal;
    }
  }

  return res;
}

template <>
Matrix<> threshold<ARCV_THRESH_TYPE_HYSTERESIS_AUTO>(const Matrix<>& mat, std::initializer_list<float> lowerBounds,
                                                                          std::initializer_list<float> upperBounds) {
  const float deriv = mat.computeStandardDeviations()[0];

  return threshold<ARCV_THRESH_TYPE_HYSTERESIS>(mat, { deriv }, { deriv + 30 });
}

} // namespace Image

} // namespace Arcv
