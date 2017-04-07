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
      case ARCV_COLORSPACE_GRAY:
        int color;
        for (auto it = mat.getData().begin(); it != mat.getData().end(); it += channels)
          color = std::accumulate(it, it + channels, 0) / channels;

        // TODO: fill the vector with color
        mat.getData().resize(mat.getData().size() / channels);
        break;

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
