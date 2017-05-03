#include <iostream>

#include "ArcV/Math/Matrix.hpp"
#include "ArcV/Processing/Image.hpp"

namespace Arcv {

template <>
void Matrix<uint8_t>::convolve(const Matrix<float>& convMat) {
  assert(("Error: Convolution matrix must be a square one", convMat.getWidth() == convMat.getHeight()));
  assert(("Error: Convolution matrix's size must be odd", convMat.getData().size() % 2 == 1));

  // TODO: make this work for any number of channels
  //Image::changeColorspace<ARCV_COLORSPACE_GRAY>(*this);
  const Mat tempMat = *this;

  const uint8_t channels = Image::getChannelCount(tempMat);

  for (unsigned int matHeightIndex = 0; matHeightIndex < tempMat.getHeight(); ++matHeightIndex) {
    for (unsigned int matWidthIndex = 0; matWidthIndex < tempMat.getWidth() * channels; matWidthIndex += channels) {
      const unsigned int matIndex = (matHeightIndex * tempMat.getWidth()) * channels + matWidthIndex;
      float value = 0;

      for (unsigned int convHeightIndex = 0; convHeightIndex < convMat.getHeight(); ++convHeightIndex) {
        for (unsigned int convWidthIndex = 0; convWidthIndex < convMat.getWidth(); ++convWidthIndex) {
          const unsigned int correspHeight = matHeightIndex - convHeightIndex;
          const unsigned int correspWidth = matWidthIndex - convWidthIndex;

          for (uint8_t chan = 0; chan < channels; ++chan) {
            //if (correspHeight > matHeightIndex || correspWidth > matWidthIndex) {
            if (correspHeight > tempMat.getHeight() || correspWidth > tempMat.getWidth() * channels) {
              // TODO: accumulate the nearest correct value
              value = 0;
            } else {
              value += convMat[convHeightIndex * convMat.getWidth() + convWidthIndex]
                * tempMat[correspHeight * tempMat.getWidth() + correspWidth + chan];
            }

            data[matIndex] = value;
          }
        }
      }
    }
  }
}

} // namespace Arcv
