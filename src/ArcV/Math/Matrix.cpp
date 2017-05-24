#include "ArcV/Math/Matrix.hpp"
#include "ArcV/Processing/Image.hpp"

namespace Arcv {

template <>
void Matrix<>::convolve(const Matrix<float>& convMat) {
  assert(("Error: Convolution matrix must be a square one", convMat.getWidth() == convMat.getHeight()));
  assert(("Error: Convolution matrix's size must be odd", convMat.getData().size() % 2 == 1));

  const Matrix<> tempMat = *this;
  const int minConvIndex = -((convMat.getWidth() - 1) / 2), maxConvIndex = (convMat.getWidth() - 1) / 2;

  for (unsigned int matHeightIndex = 0; matHeightIndex < tempMat.getHeight(); ++matHeightIndex) {
    for (unsigned int matWidthIndex = 0; matWidthIndex < tempMat.getWidth(); ++matWidthIndex) {
      for (uint8_t chan = 0; chan < tempMat.getChannels(); ++chan) {
        float value = 0.f;

        for (int convHeightIndex = minConvIndex; convHeightIndex <= maxConvIndex; ++convHeightIndex) {
          for (int convWidthIndex = minConvIndex; convWidthIndex <= maxConvIndex; ++convWidthIndex) {
            const unsigned int correspHeightIndex = matHeightIndex + convHeightIndex;
            const unsigned int correspWidthIndex = matWidthIndex + convWidthIndex;

            if (correspHeightIndex < 0 || correspHeightIndex > tempMat.getHeight()
                || correspWidthIndex < 0 || correspWidthIndex > tempMat.getWidth()) {
              value += 0.f;
            } else {
              value += convMat[(convHeightIndex + maxConvIndex) * convMat.getWidth() + convWidthIndex + maxConvIndex]
                * tempMat[(correspHeightIndex * tempMat.getWidth() + correspWidthIndex) * channels + chan];
            }
          }
        }

        data[(matHeightIndex * tempMat.getWidth() + matWidthIndex) * channels + chan] = value;
      }
    }
  }
}

} // namespace Arcv
