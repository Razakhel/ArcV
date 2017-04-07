#include <numeric>

namespace Arcv {

template <Colorspace C>
void Image::changeColorspace(Mat& mat) {
  if (C != mat.getColorspace()) {
    unsigned short channels;

    switch (mat.getColorspace()) {
      case ARCV_COLORSPACE_GRAY:
        channels = 1;
        break;

      case ARCV_COLORSPACE_GRAY_ALPHA:
        channels = 2;
        break;

      case ARCV_COLORSPACE_RGB:
      case ARCV_COLORSPACE_HSV:
      default:
        channels = 3;
        break;

      case ARCV_COLORSPACE_RGBA:
        channels = 4;
        break;
    }

    switch (C) {
      case ARCV_COLORSPACE_GRAY: {
        unsigned int index = 0;
        // Avoid
        const uint8_t alpha = static_cast<uint8_t>(mat.getColorspace() == ARCV_COLORSPACE_GRAY_ALPHA
                                                   || mat.getColorspace() == ARCV_COLORSPACE_RGBA ? 1 : 0);

        for (auto it = mat.getData().begin(); it != mat.getData().end(); it += channels, ++index)
          mat.getData()[index] = static_cast<unsigned char>(std::accumulate(it, it + channels - alpha, 0) / channels);

        mat.getData().resize(mat.getData().size() / channels);
        mat.setColorspace(C);
        break;
      }

      case ARCV_COLORSPACE_GRAY_ALPHA:

        break;

      case ARCV_COLORSPACE_RGB:

        break;

      case ARCV_COLORSPACE_HSV:

        break;

      case ARCV_COLORSPACE_RGBA:

        break;
    }
  }
}

} // namespace Arcv
