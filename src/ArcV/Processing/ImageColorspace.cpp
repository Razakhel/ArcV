#include "ArcV/Processing/Image.hpp"

namespace Arcv {

namespace Image {

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

} // namespace

template <>
Matrix<> changeColorspace<ARCV_COLORSPACE_GRAY>(Matrix<> mat) {
  std::size_t index = 0;
  // Avoiding alpha channel, not including it into the operation
  const uint8_t alpha = static_cast<uint8_t>(mat.getColorspace() >= ARCV_COLORSPACE_GRAY_ALPHA ? 1 : 0);
  const uint8_t stride = mat.getChannelCount() - alpha;

  for (auto elt = mat.getData().begin(); elt != mat.getData().end(); elt += mat.getChannelCount(), index += 1 + alpha) {
    mat.getData()[index] = std::accumulate(elt, elt + stride, 0.f) / stride;

    if (alpha)
      mat.getData()[index + 1] = *(elt + stride);
  }

  mat.setChannelCount(static_cast<uint8_t>(1 + alpha));
  mat.getData().resize(mat.getWidth() * mat.getHeight() * mat.getChannelCount());

  if (mat.getColorspace() >= ARCV_COLORSPACE_GRAY_ALPHA)
    removeAlphaChannel(mat);
  mat.setColorspace(ARCV_COLORSPACE_GRAY);

  return mat;
}

template <>
Matrix<> changeColorspace<ARCV_COLORSPACE_RGB>(Matrix<> mat) {
  assert(("Warning: Function not handled yet",
          mat.getColorspace() == ARCV_COLORSPACE_RGBA || mat.getColorspace() == ARCV_COLORSPACE_RGB));

  if (mat.getColorspace() >= ARCV_COLORSPACE_GRAY_ALPHA)
    removeAlphaChannel(mat);
  mat.setColorspace(ARCV_COLORSPACE_RGB);

  return mat;
}

template <>
Matrix<> changeColorspace<ARCV_COLORSPACE_HSV>(Matrix<> mat) {
  assert(("Error: Input matrix's colorspace should be RGB(A)",
          mat.getColorspace() == ARCV_COLORSPACE_RGB || mat.getColorspace() == ARCV_COLORSPACE_RGBA));

  for (auto elt = mat.getData().begin(); elt != mat.getData().end(); elt += mat.getChannelCount()) {
    const float red = *elt / 255;
    const float green = *(elt + 1) / 255;
    const float blue = *(elt + 2) / 255;

    const float minVal = std::min(std::min(red, green), blue);
    const float maxVal = std::max(std::max(red, green), blue);
    const float delta = maxVal - minVal;

    // Hue
    float hue = 0.f;

    if (delta == 0) {
      *elt = 0;
    } else {
      if (maxVal == red) {
        hue = 60 * (green - blue) / delta;

        if (hue < 0.f)
          hue += 360;
      } else if (maxVal == green) {
        hue = 120 + 60 * (blue - red) / delta;
      } else if (maxVal == blue) {
        hue = 240 + 60 * (red - green) / delta;
      }

      *elt = hue / 2;
    }

    // Saturation
    *(elt + 1) = (maxVal == 0 ? 0 : delta / maxVal * 255);

    // Value
    *(elt + 2) = maxVal * 255;
  }

  if (mat.getColorspace() >= ARCV_COLORSPACE_GRAY_ALPHA)
    removeAlphaChannel(mat);
  mat.setColorspace(ARCV_COLORSPACE_HSV);

  return mat;
}

template <>
Matrix<> changeColorspace<ARCV_COLORSPACE_GRAY_ALPHA>(Matrix<> mat) {
  mat = changeColorspace<ARCV_COLORSPACE_GRAY>(mat);

  if (mat.getColorspace() < ARCV_COLORSPACE_GRAY_ALPHA)
    addAlphaChannel(mat);
  mat.setColorspace(ARCV_COLORSPACE_GRAY_ALPHA);

  return mat;
}

template <>
Matrix<> changeColorspace<ARCV_COLORSPACE_RGBA>(Matrix<> mat) {
  assert(("Warning: Function not handled yet",
          mat.getColorspace() == ARCV_COLORSPACE_RGB || mat.getColorspace() == ARCV_COLORSPACE_RGBA));

  if (mat.getColorspace() < ARCV_COLORSPACE_GRAY_ALPHA)
    addAlphaChannel(mat);
  mat.setColorspace(ARCV_COLORSPACE_RGBA);

  return mat;
}

} // namespace Image

} // namespace Arcv
