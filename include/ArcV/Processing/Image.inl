#include <numeric>
#include <cassert>

namespace Arcv {

namespace {

void removeAlphaChannel(Mat& mat, const uint8_t channels) {
  for (unsigned int i = channels, delta = 1; i < mat.getData().size(); i += channels, ++delta) {
    for (unsigned int chan = i; chan < i + channels - 1; ++chan)
      mat.getData()[chan - delta] = mat.getData()[chan];
  }

  mat.getData().resize(mat.getData().size() * (channels - 1) / channels);
}

void convertToGrayscale(Mat& mat, const uint8_t channels) {
  unsigned int index = 0;
  // Avoiding alpha channel, not including it into the operation
  const uint8_t alpha = static_cast<uint8_t>(mat.getColorspace() == ARCV_COLORSPACE_GRAY_ALPHA
                                             || mat.getColorspace() == ARCV_COLORSPACE_RGBA ? 1 : 0);

  for (auto it = mat.getData().begin(); it != mat.getData().end(); it += channels, ++index)
    mat.getData()[index] = static_cast<unsigned char>(std::accumulate(it, it + channels - alpha, 0) / (channels - alpha));

  mat.getData().resize(mat.getData().size() / channels);
}

void convertToHSV(Mat& mat) {
  uint8_t red, green, blue, minVal, maxVal;
  float hue = 0.f;

  for (unsigned int i = 0; i < mat.getData().size(); i += 3) {
    red = mat.getData()[i];
    green = mat.getData()[i + 1];
    blue = mat.getData()[i + 2];

    minVal = std::min(std::min(red, green), blue);
    maxVal = std::max(std::max(red, green), blue);

    // Hue
    if (maxVal == minVal) {
      mat.getData()[i] = 0;
    } else {
      if (maxVal == red)
        hue = (60 * ((green - blue) / (maxVal - minVal)) + 360) % 360;
      else if (maxVal == green)
        hue = 60 * ((blue - red) / (maxVal - minVal)) + 120;
      else if (maxVal == blue)
        hue = 60 * ((red - green) / (maxVal - minVal)) + 240;

      mat.getData()[i] = static_cast<unsigned char>(hue / 360.f * 255);
    }

    // Saturation
    mat.getData()[i + 1] = static_cast<unsigned char>(maxVal == 0 ? 0 : 1 - (minVal / maxVal));

    // Value
    mat.getData()[i + 2] = maxVal;
  }
}

} // namespace

template <Colorspace C>
void Image::changeColorspace(Mat& mat) {
  if (C != mat.getColorspace()) {
    uint8_t channels;

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
        if (mat.getColorspace() == ARCV_COLORSPACE_GRAY_ALPHA)
          removeAlphaChannel(mat, channels);
        else
          convertToGrayscale(mat, channels);
        break;

      case ARCV_COLORSPACE_GRAY_ALPHA:

        break;

      case ARCV_COLORSPACE_RGB:
        if (mat.getColorspace() == ARCV_COLORSPACE_RGBA)
          removeAlphaChannel(mat, channels);
        break;

      case ARCV_COLORSPACE_HSV:
        assert(("Error: Input matrix's colorspace should be RGB(A)",
                mat.getColorspace() == ARCV_COLORSPACE_RGB || mat.getColorspace() == ARCV_COLORSPACE_RGBA));

        if (mat.getColorspace() == ARCV_COLORSPACE_RGBA)
          removeAlphaChannel(mat, channels);

        convertToHSV(mat);
        break;

      case ARCV_COLORSPACE_RGBA:

        break;
    }

    mat.setColorspace(C);
  }
}

} // namespace Arcv
