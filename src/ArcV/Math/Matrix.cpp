#include "ArcV/Math/Matrix.hpp"
#include "ArcV/Processing/Image.hpp"

namespace Arcv {

template <>
Matrix<> Matrix<>::convolve(const Matrix<float>& convMat) {
  assert(("Error: Convolution matrix must be a square one", convMat.getWidth() == convMat.getHeight()));
  assert(("Error: Convolution matrix's size must be odd", convMat.getData().size() % 2 == 1));

  Matrix<> tempMat = *this;
  const int minConvIndex = -((static_cast<int>(convMat.getWidth()) - 1) / 2);
  const int maxConvIndex = (static_cast<int>(convMat.getWidth()) - 1) / 2;

  for (unsigned int matHeightIndex = 0; matHeightIndex < height; ++matHeightIndex) {
    for (unsigned int matWidthIndex = 0; matWidthIndex < width; ++matWidthIndex) {
      for (uint8_t chan = 0; chan < channelCount; ++chan) {
        float value = 0.f;

        for (int convHeightIndex = minConvIndex; convHeightIndex <= maxConvIndex; ++convHeightIndex) {
          for (int convWidthIndex = minConvIndex; convWidthIndex <= maxConvIndex; ++convWidthIndex) {
            const unsigned int correspHeightIndex = matHeightIndex + convHeightIndex;
            const unsigned int correspWidthIndex = matWidthIndex + convWidthIndex;

            // No need to check if negative since it is unsigned
            if (correspHeightIndex < height && correspWidthIndex < width) {
              value += convMat[(convHeightIndex + maxConvIndex) * convMat.getWidth() + convWidthIndex + maxConvIndex]
                  * data[(correspHeightIndex * width + correspWidthIndex) * channelCount + chan];
            }
          }
        }

        tempMat.getData()[(matHeightIndex * width + matWidthIndex) * channelCount + chan] = value;
      }
    }
  }

  return tempMat;
}

} // namespace Arcv
