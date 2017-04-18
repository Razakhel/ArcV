#include <iostream>

#include "ArcV/Math/Matrix.hpp"
#include "ArcV/Processing/Image.hpp"

namespace Arcv {

template <>
void Matrix<uint8_t>::convolve(const Matrix<float>& convMat) {
  // TODO: handle out of range values
  const uint8_t channels = Image::getChannelCount(*this);

  for (unsigned int h = 0; h < data.size() / width; ++h) {
    for (unsigned int w = 0; w < data.size() / height; w += channels) {
      for (unsigned int chan = 0; chan < channels; ++chan) {
        // TODO: process value with convolution matrix
        data[h * width + w + chan] = 255;
      }
    }
  }
}

} // namespace Arcv
