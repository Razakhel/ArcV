#include <iostream>

#include "ArcV/Math/Matrix.hpp"
#include "ArcV/Processing/Image.hpp"

namespace Arcv {

template <>
void Matrix<uint8_t>::convolve(const Matrix<float>& convMat) {
  assert(("Error: Convolution matrix must be a square one", convMat.getWidth() == convMat.getHeight()));
  assert(("Error: Convolution matrix's size must be odd", convMat.getData().size() % 2 == 1));

  // TODO: make this work for any number of channels
  const Mat tempMat = *this;

  const uint8_t channels = Image::getChannelCount(tempMat);

  for (unsigned int matHeightIndex = 0; matHeightIndex < tempMat.getHeight(); ++matHeightIndex) {
    for (unsigned int matWidthIndex = 0; matWidthIndex < tempMat.getWidth() * channels; matWidthIndex += channels) {
      const unsigned int matIndex = (matHeightIndex * tempMat.getWidth()) * channels + matWidthIndex;

      for (int convHeightIndex = -(convMat.getData().size() - 1) / 2; convHeightIndex < (convMat.getData().size() - 1) / 2; ++convHeightIndex) {
        for (int convWidthIndex = -(convMat.getData().size() - 1) / 2; convWidthIndex < (convMat.getData().size() - 1) / 2; ++convWidthIndex) {
          const unsigned int correspHeight = matHeightIndex + convHeightIndex;
          const unsigned int correspWidth = matWidthIndex + convWidthIndex;

          for (uint8_t chan = 0; chan < channels; ++chan) {
            float value = 0.f;

            //if (correspHeight > matHeightIndex || correspWidth > matWidthIndex) {
            if (correspHeight > tempMat.getHeight() || correspWidth > tempMat.getWidth() * channels) {
              // TODO: accumulate the nearest correct value
              value = 0;
            } else {
              value += convMat[convHeightIndex * convMat.getWidth() + convWidthIndex]
                * tempMat[(correspHeight * tempMat.getWidth()) * channels + correspWidth + chan];
            }

            data[matIndex + chan] = value;
          }
        }
      }
    }
  }
}

} // namespace Arcv
