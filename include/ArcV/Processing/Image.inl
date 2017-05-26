#include <numeric>
#include <cassert>

namespace Arcv {

namespace {

void addAlphaChannel(Matrix<>& mat) {
  // TODO: implement function
}

void removeAlphaChannel(Matrix<>& mat) {
  for (unsigned int i = mat.getChannelCount(), delta = 1; i < mat.getData().size(); i += mat.getChannelCount(), ++delta) {
    for (unsigned int chan = i; chan < i + mat.getChannelCount() - 1; ++chan)
      mat.getData()[chan - delta] = mat.getData()[chan];
  }

  mat.getData().resize(mat.getData().size() * (mat.getChannelCount() - 1) / mat.getChannelCount());
  mat.setChannelCount(mat.getChannelCount() - 1);
}

void convertToGrayscale(Matrix<>& mat) {
  unsigned int index = 0;
  // Avoiding alpha channel, not including it into the operation
  const uint8_t stride = static_cast<uint8_t>(mat.getChannelCount() - (mat.getColorspace() == ARCV_COLORSPACE_GRAY_ALPHA
                                                                   || mat.getColorspace() == ARCV_COLORSPACE_RGBA ? 1 : 0));

  for (auto it = mat.getData().begin(); it != mat.getData().end(); it += mat.getChannelCount(), ++index)
    mat.getData()[index] = std::accumulate(it, it + stride, 0) / (stride);

  mat.getData().resize(mat.getData().size() / mat.getChannelCount());
  mat.setChannelCount(1);
}

void convertToHSV(Matrix<>& mat) {
  float red, green, blue, minVal, maxVal, hue = 0.f;

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
        hue = std::fmod(60 * ((green - blue) / (maxVal - minVal)) + 360, 360.f);
      else if (maxVal == green)
        hue = 60 * ((blue - red) / (maxVal - minVal)) + 120;
      else if (maxVal == blue)
        hue = 60 * ((red - green) / (maxVal - minVal)) + 240;

      mat.getData()[i] = hue / 2;
    }

    // Saturation
    mat.getData()[i + 1] = maxVal == 0 ? 0 : 1 - (minVal / maxVal);

    // Value
    mat.getData()[i + 2] = maxVal;
  }

  mat.setChannelCount(3);
}

void applyGaussianBlur(Matrix<>& mat) {
  Matrix<float> kernel = {{ 1.f,  4.f,  6.f,  4.f, 1.f },
                          { 4.f, 16.f, 24.f, 16.f, 4.f },
                          { 6.f, 24.f, 36.f, 24.f, 6.f },
                          { 4.f, 16.f, 24.f, 16.f, 4.f },
                          { 1.f,  4.f,  6.f,  4.f, 1.f }};

  mat.convolve(kernel / 256);
}

void applySharpen(Matrix<>& mat) {
  Matrix<float> kernel = {{ 0.f,  -1.f,  0.f },
                          { -1.f,  5.f, -1.f },
                          { 0.f,  -1.f,  0.f }};

  mat.convolve(kernel);
}

void applyEdgeDetection(Matrix<>& mat) {
  Matrix<float> kernel = {{ 0.f,   1.f,  0.f },
                          { 1.f,  -4.f,  1.f },
                          { 0.f,   1.f,  0.f }};

  mat.convolve(kernel);
}

void applyEmboss(Matrix<>& mat) {
  Matrix<float> kernel = {{ -2.f, -1.f, 0.f },
                          { -1.f,  1.f, 1.f },
                          {  0.f,  1.f, 2.f }};

  mat.convolve(kernel);
}

} // namespace

template <Colorspace C>
void Image::changeColorspace(Matrix<>& mat) {
  if (C != mat.getColorspace()) {
    switch (C) {
      case ARCV_COLORSPACE_GRAY:
        if (mat.getColorspace() == ARCV_COLORSPACE_GRAY_ALPHA)
          removeAlphaChannel(mat);
        else
          convertToGrayscale(mat);
        break;

      case ARCV_COLORSPACE_GRAY_ALPHA:
        if (mat.getColorspace() == ARCV_COLORSPACE_GRAY)
          addAlphaChannel(mat);
        break;

      case ARCV_COLORSPACE_RGB:
        if (mat.getColorspace() == ARCV_COLORSPACE_RGBA)
          removeAlphaChannel(mat);
        break;

      case ARCV_COLORSPACE_HSV:
        assert(("Error: Input matrix's colorspace should be RGB(A)",
                mat.getColorspace() == ARCV_COLORSPACE_RGB || mat.getColorspace() == ARCV_COLORSPACE_RGBA));

        if (mat.getColorspace() == ARCV_COLORSPACE_RGBA)
          removeAlphaChannel(mat);

        convertToHSV(mat);
        break;

      case ARCV_COLORSPACE_RGBA:
        if (mat.getColorspace() == ARCV_COLORSPACE_RGB)
          addAlphaChannel(mat);
        break;
    }

    mat.setColorspace(C);
  }
}

template <FilterType F>
void Image::applyFilter(Matrix<>& mat) {
  switch (F) {
    case ARCV_FILTER_TYPE_GAUSSIAN_BLUR:
      applyGaussianBlur(mat);
      break;

    case ARCV_FILTER_TYPE_SHARPEN:
      applySharpen(mat);
      break;

    case ARCV_FILTER_TYPE_EDGE_DETECTION:
      applyEdgeDetection(mat);
      break;

    case ARCV_FILTER_TYPE_EMBOSS:
      applyEmboss(mat);
      break;

    default:
      break;
  }
}

} // namespace Arcv
