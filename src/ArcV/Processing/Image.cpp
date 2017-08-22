#include <array>
#include <vector>
#include <fstream>

#include "png/png.h"
#include "png/zlib.h"

#include "ArcV/Processing/Image.hpp"

namespace Arcv {

namespace {

bool validatePng(std::istream& file) {
  std::array<png_byte, PNG_HEADER_SIZE> header;
  file.read(reinterpret_cast<char*>(header.data()), PNG_HEADER_SIZE);

  return (png_sig_cmp(header.data(), 0, PNG_HEADER_SIZE) == 0);
}

void readPng(png_structp pngReadPtr, png_bytep data, png_size_t length) {
  png_voidp inPtr = png_get_io_ptr(pngReadPtr);
  static_cast<std::istream*>(inPtr)->read(reinterpret_cast<char*>(data), length);
}

void writePng(png_structp pngWritePtr, png_bytep data, png_size_t length) {
  png_voidp outPtr = png_get_io_ptr(pngWritePtr);
  static_cast<std::ostream*>(outPtr)->write(reinterpret_cast<const char*>(data), length);
}

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

} // namespace

Matrix<> Image::read(const std::string& fileName) {
  std::ifstream file(fileName, std::ios_base::in | std::ios_base::binary);
  const bool valid = file.good() && validatePng(file);
  assert(("Error: Not a valid PNG", valid));

  png_structp pngReadStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  assert(("Error: Couldn't initialize PNG read struct", pngReadStruct));

  png_infop pngInfoStruct = png_create_info_struct(pngReadStruct);
  assert(("Error: Couldn't initialize PNG info struct", pngInfoStruct));

  png_set_read_fn(pngReadStruct, &file, readPng);

  // Setting the amount signature bytes we've already read
  png_set_sig_bytes(pngReadStruct, PNG_HEADER_SIZE);

  png_read_info(pngReadStruct, pngInfoStruct);

  uint32_t width = static_cast<uint32_t>(png_get_image_width(pngReadStruct, pngInfoStruct));
  uint32_t height = static_cast<uint32_t>(png_get_image_height(pngReadStruct, pngInfoStruct));
  uint32_t bitDepth = png_get_bit_depth(pngReadStruct, pngInfoStruct);
  uint8_t channels = png_get_channels(pngReadStruct, pngInfoStruct);
  uint8_t colorType = png_get_color_type(pngReadStruct, pngInfoStruct);
  Colorspace colorspace;

  switch (colorType) {
    case PNG_COLOR_TYPE_GRAY:
      if (bitDepth < 8)
        png_set_expand_gray_1_2_4_to_8(pngReadStruct);
      colorspace = ARCV_COLORSPACE_GRAY;
      bitDepth = 8;
      break;

    case PNG_COLOR_TYPE_GRAY_ALPHA:
      colorspace = ARCV_COLORSPACE_GRAY_ALPHA;
      break;

    case PNG_COLOR_TYPE_PALETTE:
      png_set_palette_to_rgb(pngReadStruct);
      colorspace = ARCV_COLORSPACE_RGB;
      channels = 3;
      break;

    case PNG_COLOR_TYPE_RGB:
    default:
      colorspace = ARCV_COLORSPACE_RGB;
      break;

    case PNG_COLOR_TYPE_RGB_ALPHA:
      colorspace = ARCV_COLORSPACE_RGBA;
      break;
  }

  png_set_scale_16(pngReadStruct);

  // Adding full alpha channel to the image if it possesses transparency
  if (png_get_valid(pngReadStruct, pngInfoStruct, static_cast<png_uint_32>(PNG_INFO_tRNS))) {
    png_set_tRNS_to_alpha(pngReadStruct);
    ++channels;
  }

  png_read_update_info(pngReadStruct, pngInfoStruct);

  Matrix<uint8_t> mat(width, height, channels, static_cast<uint8_t>(bitDepth), colorspace);

  std::vector<png_bytep> rowPtrs(height);

  // Mapping row's elements to data's
  for (unsigned int i = 0; i < height; ++i)
    rowPtrs[i] = &mat.getData()[width * channels * i];

  png_read_image(pngReadStruct, rowPtrs.data());
  png_read_end(pngReadStruct, pngInfoStruct);
  png_destroy_read_struct(&pngReadStruct, nullptr, &pngInfoStruct);

  return Matrix<>(mat);
}

void Image::write(const Matrix<>& mat, const std::string& fileName) {
  std::ofstream file(fileName, std::ios_base::out | std::ios_base::binary);
  const Matrix<uint8_t> matToWrite(mat);

  png_structp pngWriteStruct = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  assert(("Error: Couldn't initialize PNG write struct", pngWriteStruct));

  png_infop pngInfoStruct = png_create_info_struct(pngWriteStruct);
  assert(("Error: Couldn't initialize PNG info struct", pngInfoStruct));

  uint32_t colorType;
  switch (matToWrite.getColorspace()) {
    case ARCV_COLORSPACE_GRAY:
      colorType = PNG_COLOR_TYPE_GRAY;
      break;

    case ARCV_COLORSPACE_GRAY_ALPHA:
      colorType = PNG_COLOR_TYPE_GRAY_ALPHA;
      break;

    case ARCV_COLORSPACE_RGB:
    case ARCV_COLORSPACE_HSV:
    default:
      colorType = PNG_COLOR_TYPE_RGB;
      break;

    case ARCV_COLORSPACE_RGBA:
      colorType = PNG_COLOR_TYPE_RGBA;
      break;
  }

  png_set_compression_level(pngWriteStruct, 6);

  if (matToWrite.getChannelCount() * matToWrite.getImgBitDepth() >= 16) {
    png_set_compression_strategy(pngWriteStruct, Z_FILTERED);
    png_set_filter(pngWriteStruct, 0, PNG_FILTER_NONE | PNG_FILTER_SUB | PNG_FILTER_PAETH);
  } else {
    png_set_compression_strategy(pngWriteStruct, Z_DEFAULT_STRATEGY);
  }

  png_set_IHDR(pngWriteStruct,
               pngInfoStruct,
               static_cast<png_uint_32>(matToWrite.getWidth()),
               static_cast<png_uint_32>(matToWrite.getHeight()),
               matToWrite.getImgBitDepth(),
               colorType,
               PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_BASE,
               PNG_FILTER_TYPE_BASE);

  png_set_write_fn(pngWriteStruct, &file, writePng, nullptr);
  png_write_info(pngWriteStruct, pngInfoStruct);

  for (unsigned int i = 0; i < matToWrite.getHeight(); ++i)
    png_write_row(pngWriteStruct, &matToWrite.getData()[matToWrite.getWidth() * matToWrite.getChannelCount() * i]);

  png_write_end(pngWriteStruct, pngInfoStruct);
  png_destroy_write_struct(&pngWriteStruct, &pngInfoStruct);
}

template <>
Matrix<> Image::changeColorspace<ARCV_COLORSPACE_GRAY>(Matrix<> mat) {
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
Matrix<> Image::changeColorspace<ARCV_COLORSPACE_RGB>(Matrix<> mat) {
  assert(("Warning: Function not handled yet",
          mat.getColorspace() == ARCV_COLORSPACE_RGBA || mat.getColorspace() == ARCV_COLORSPACE_RGB));

  if (mat.getColorspace() >= ARCV_COLORSPACE_GRAY_ALPHA)
    removeAlphaChannel(mat);
  mat.setColorspace(ARCV_COLORSPACE_RGB);

  return mat;
}

template <>
Matrix<> Image::changeColorspace<ARCV_COLORSPACE_HSV>(Matrix<> mat) {
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
Matrix<> Image::changeColorspace<ARCV_COLORSPACE_GRAY_ALPHA>(Matrix<> mat) {
  mat = changeColorspace<ARCV_COLORSPACE_GRAY>(mat);

  if (mat.getColorspace() < ARCV_COLORSPACE_GRAY_ALPHA)
    addAlphaChannel(mat);
  mat.setColorspace(ARCV_COLORSPACE_GRAY_ALPHA);

  return mat;
}

template <>
Matrix<> Image::changeColorspace<ARCV_COLORSPACE_RGBA>(Matrix<> mat) {
  assert(("Warning: Function not handled yet",
          mat.getColorspace() == ARCV_COLORSPACE_RGB || mat.getColorspace() == ARCV_COLORSPACE_RGBA));

  if (mat.getColorspace() < ARCV_COLORSPACE_GRAY_ALPHA)
    addAlphaChannel(mat);
  mat.setColorspace(ARCV_COLORSPACE_RGBA);

  return mat;
}

template <>
Matrix<> Image::applyFilter<ARCV_FILTER_TYPE_GAUSSIAN_BLUR>(Matrix<> mat) {
  Matrix<float> kernel = {{ 1.f,  4.f,  6.f,  4.f, 1.f },
                          { 4.f, 16.f, 24.f, 16.f, 4.f },
                          { 6.f, 24.f, 36.f, 24.f, 6.f },
                          { 4.f, 16.f, 24.f, 16.f, 4.f },
                          { 1.f,  4.f,  6.f,  4.f, 1.f }};

  return mat.convolve(kernel / 256);
}

template <>
Matrix<> Image::applyFilter<ARCV_FILTER_TYPE_SHARPEN>(Matrix<> mat) {
  const Matrix<float> kernel = {{  0.f,  -1.f,  0.f },
                                { -1.f,   5.f, -1.f },
                                {  0.f,  -1.f,  0.f }};

  return mat.convolve(kernel);
}

template <>
Matrix<> Image::applyFilter<ARCV_FILTER_TYPE_EDGE_ENHANCEMENT>(Matrix<> mat) {
  const Matrix<float> kernel = {{ 0.f,   1.f,  0.f },
                                { 1.f,  -4.f,  1.f },
                                { 0.f,   1.f,  0.f }};

  return mat.convolve(kernel);
}

template <>
Matrix<> Image::applyFilter<ARCV_FILTER_TYPE_EMBOSS>(Matrix<> mat) {
  const Matrix<float> kernel = {{ -2.f, -1.f, 0.f },
                                { -1.f,  1.f, 1.f },
                                {  0.f,  1.f, 2.f }};

  return mat.convolve(kernel);
}

template <>
Matrix<> Image::applyFilter<ARCV_FILTER_TYPE_SOBEL>(Matrix<> mat) {
  const Matrix<float> horizRes = computeHorizontalSobelOperator(mat);
  const Matrix<float> vertRes = computeVerticalSobelOperator(mat);

  for (std::size_t i = 0; i < mat.getData().size(); ++i)
    mat.getData()[i] = std::sqrt(vertRes.getData()[i] * vertRes.getData()[i]
                                 + horizRes.getData()[i] * horizRes.getData()[i]);

  return mat;
}

template <>
Matrix<> Image::applyDetector<ARCV_DETECTOR_TYPE_HARRIS>(Matrix<> mat) {
  mat = changeColorspace<ARCV_COLORSPACE_GRAY>(mat);

  Matrix<float> horizRes = computeHorizontalSobelOperator(mat);
  Matrix<float> vertRes = computeVerticalSobelOperator(mat);

  Matrix<float> res = horizRes * vertRes;
  horizRes *= horizRes;
  vertRes *= vertRes;

  for (std::size_t i = 0; i < mat.getData().size(); ++i) {
    if ((horizRes.getData()[i] * vertRes.getData()[i] - res.getData()[i] * res.getData()[i])
        - 0.04f * std::pow(horizRes.getData()[i] + vertRes.getData()[i], 2.f) > 255)
      mat.getData()[i] = 255;
  }

  return mat;
}

} // namespace Arcv
