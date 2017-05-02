#include <numeric>
#include <cassert>

namespace Arcv {

namespace {

void addAlphaChannel(Mat& mat, const uint8_t channels) {
  // TODO: implement function
}

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
    mat.getData()[index] = static_cast<uint8_t>(std::accumulate(it, it + channels - alpha, 0) / (channels - alpha));

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

      mat.getData()[i] = static_cast<uint8_t>(hue / 360.f * 255);
    }

    // Saturation
    mat.getData()[i + 1] = static_cast<uint8_t>(maxVal == 0 ? 0 : 1 - (minVal / maxVal));

    // Value
    mat.getData()[i + 2] = maxVal;
  }
}

void applyGaussianBlur(Matrix<uint8_t>& mat) {
  Matrix<float> kernel = {{ 1.f, 1.f, 1.f, 1.f, 1.f },
                          { 1.f, 1.f, 1.f, 1.f, 1.f },
                          { 1.f, 1.f, 1.f, 1.f, 1.f },
                          { 1.f, 1.f, 1.f, 1.f, 1.f },
                          { 1.f, 1.f, 1.f, 1.f, 1.f }};

  mat.convolve(kernel / kernel.getData().size());
}

} // namespace

template <Colorspace C>
void Image::changeColorspace(Mat& mat) {
  if (C != mat.getColorspace()) {
    const uint8_t channels = getChannelCount(mat);

    switch (C) {
      case ARCV_COLORSPACE_GRAY:
        if (mat.getColorspace() == ARCV_COLORSPACE_GRAY_ALPHA)
          removeAlphaChannel(mat, channels);
        else
          convertToGrayscale(mat, channels);
        break;

      case ARCV_COLORSPACE_GRAY_ALPHA:
        if (mat.getColorspace() == ARCV_COLORSPACE_GRAY)
          addAlphaChannel(mat, channels);
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
        if (mat.getColorspace() == ARCV_COLORSPACE_RGB)
          addAlphaChannel(mat, channels);
        break;
    }

    mat.setColorspace(C);
  }
}

template <FilterType F>
void Image::applyFilter(Matrix<uint8_t>& mat) {
  switch (F) {
    case ARCV_FILTER_TYPE_GAUSSIAN_BLUR:
      applyGaussianBlur(mat);
      break;

    default:
      break;
  }
}

} // namespace Arcv
