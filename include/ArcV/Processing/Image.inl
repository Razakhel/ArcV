#include <numeric>
#include <cassert>

namespace Arcv {

namespace {

void addAlphaChannel(Matrix<>& mat) {
  const Matrix<float> tempMat = mat;

  mat.setChannelCount(static_cast<uint8_t>(mat.getChannelCount() + 1));
  mat.getData().resize(mat.getWidth() * mat.getHeight() * mat.getChannelCount());

  for (std::size_t i = 0, delta = 0; i < tempMat.getData().size(); i += tempMat.getChannelCount(), ++delta) {
    for (std::size_t chan = i; chan < i + tempMat.getChannelCount(); ++chan)
      mat.getData()[chan + delta] = tempMat.getData()[chan];

    // Filling the alpha value with full opacity by default
    mat.getData()[i + tempMat.getChannelCount() + delta] = 255;
  }
}

void removeAlphaChannel(Matrix<>& mat) {
  // No need to start at the very beginning since the first channel pack will not be moved
  for (std::size_t i = mat.getChannelCount(), delta = 1; i < mat.getData().size(); i += mat.getChannelCount(), ++delta) {
    for (std::size_t chan = i; chan < i + mat.getChannelCount() - 1; ++chan)
      mat.getData()[chan - delta] = mat.getData()[chan];
  }

  mat.setChannelCount(static_cast<uint8_t>(mat.getChannelCount() - 1));
  mat.getData().resize(mat.getWidth() * mat.getHeight() * mat.getChannelCount());
}

void convertToGrayscale(Matrix<>& mat) {
  unsigned int index = 0;
  // Avoiding alpha channel, not including it into the operation
  const uint8_t alpha = static_cast<uint8_t>(mat.getColorspace() >= ARCV_COLORSPACE_GRAY_ALPHA ? 1 : 0);
  const uint8_t stride = mat.getChannelCount() - alpha;

  for (auto element = mat.getData().begin(); element != mat.getData().end(); element += mat.getChannelCount(), index += 1 + alpha) {
    mat.getData()[index] = std::accumulate(element, element + stride, 0) / stride;

    if (alpha)
      mat.getData()[index + 1] = *(element + stride);
  }

  mat.setChannelCount(static_cast<uint8_t>(1 + alpha));
  mat.getData().resize(mat.getWidth() * mat.getHeight() * mat.getChannelCount());
}

void convertToHSV(Matrix<>& mat) {
  float red, green, blue, minVal, maxVal, hue = 0.f;

  for (std::size_t i = 0; i < mat.getData().size(); i += mat.getChannelCount()) {
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

  mat.setChannelCount(static_cast<uint8_t>(3 + (mat.getColorspace() == ARCV_COLORSPACE_RGBA ? 1 : 0)));
}

void applyGaussianBlur(Matrix<>& mat) {
  Matrix<float> kernel = {{ 1.f,  4.f,  6.f,  4.f, 1.f },
                          { 4.f, 16.f, 24.f, 16.f, 4.f },
                          { 6.f, 24.f, 36.f, 24.f, 6.f },
                          { 4.f, 16.f, 24.f, 16.f, 4.f },
                          { 1.f,  4.f,  6.f,  4.f, 1.f }};

  mat = mat.convolve(kernel / 256);
}

void applySharpen(Matrix<>& mat) {
  const Matrix<float> kernel = {{ 0.f,  -1.f,  0.f },
                                { -1.f,  5.f, -1.f },
                                { 0.f,  -1.f,  0.f }};

  mat = mat.convolve(kernel);
}

void applyEdgeDetection(Matrix<>& mat) {
  const Matrix<float> kernel = {{ 0.f,   1.f,  0.f },
                                { 1.f,  -4.f,  1.f },
                                { 0.f,   1.f,  0.f }};

  mat = mat.convolve(kernel);
}

void applyEmboss(Matrix<>& mat) {
  const Matrix<float> kernel = {{ -2.f, -1.f, 0.f },
                                { -1.f,  1.f, 1.f },
                                {  0.f,  1.f, 2.f }};

  mat = mat.convolve(kernel);
}

Matrix<> computeHorizontalSobelOperator(Matrix<>& mat) {
  const Matrix<float> horizKernel = {{  1.f,  2.f,  1.f },
                                     {  0.f,  0.f,  0.f },
                                     { -1.f, -2.f, -1.f }};

  return mat.convolve(horizKernel);
}

Matrix<> computeVerticalSobelOperator(Matrix<>& mat) {
  const Matrix<float> vertKernel = {{ 1.f, 0.f, -1.f },
                                    { 2.f, 0.f, -2.f },
                                    { 1.f, 0.f, -1.f }};

  return mat.convolve(vertKernel);
}

void applySobel(Matrix<>& mat) {
  const Matrix<float> horizRes = computeHorizontalSobelOperator(mat);
  const Matrix<float> vertRes = computeVerticalSobelOperator(mat);

  for (std::size_t i = 0; i < mat.getData().size(); ++i)
    mat.getData()[i] = std::sqrt(vertRes.getData()[i] * vertRes.getData()[i]
                                 + horizRes.getData()[i] * horizRes.getData()[i]);
}

void applyHarris(Matrix<>& mat) {
  Image::changeColorspace<ARCV_COLORSPACE_GRAY>(mat);

  Matrix<float> horizRes = computeHorizontalSobelOperator(mat);
  Matrix<float> vertRes = computeVerticalSobelOperator(mat);

  Matrix<float> res = horizRes * vertRes;
  horizRes *= horizRes;
  vertRes *= vertRes;

  for (std::size_t i = 0; i < mat.getData(). size(); ++i)
    mat.getData()[i] = (horizRes.getData()[i] * vertRes.getData()[i] - res.getData()[i] * res.getData()[i])
                        - 0.04f * std::pow(horizRes.getData()[i] + vertRes.getData()[i], 2.f);
}

} // namespace

template <Colorspace C>
void Image::changeColorspace(Matrix<>& mat) {
  if (C != mat.getColorspace()) {
    switch (C) {
      case ARCV_COLORSPACE_GRAY:
        if (mat.getColorspace() != ARCV_COLORSPACE_GRAY_ALPHA)
          convertToGrayscale(mat);
        break;

      case ARCV_COLORSPACE_RGB:
        assert(("Warning: Function not handled yet", mat.getColorspace() == ARCV_COLORSPACE_RGBA));
        break;

      case ARCV_COLORSPACE_HSV:
        assert(("Error: Input matrix's colorspace should be RGB(A)",
                mat.getColorspace() == ARCV_COLORSPACE_RGB || mat.getColorspace() == ARCV_COLORSPACE_RGBA));
        convertToHSV(mat);
        break;

      case ARCV_COLORSPACE_GRAY_ALPHA:
        if (mat.getColorspace() != ARCV_COLORSPACE_GRAY)
          convertToGrayscale(mat);
        break;

      case ARCV_COLORSPACE_RGBA:
        assert(("Warning: Function not handled yet", mat.getColorspace() == ARCV_COLORSPACE_RGB));
        break;
    }

    if (C < ARCV_COLORSPACE_GRAY_ALPHA && mat.getColorspace() >= ARCV_COLORSPACE_GRAY_ALPHA)
      removeAlphaChannel(mat);
    else if (C >= ARCV_COLORSPACE_GRAY_ALPHA && mat.getColorspace() < ARCV_COLORSPACE_GRAY_ALPHA)
      addAlphaChannel(mat);

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

    case ARCV_FILTER_TYPE_SOBEL:
      applySobel(mat);
      break;

    default:
      break;
  }
}

template <DetectorType D>
void Image::applyDetector(Matrix<>& mat) {
  switch (D) {
    case ARCV_DETECTOR_TYPE_HARRIS:
      applyHarris(mat);
      break;

    default:
      break;
  }
}

} // namespace Arcv
