#include <array>
#include <fstream>
#include <sstream>
#include <iostream>

#include "png/png.h"
#include "png/zlib.h"
#include "ArcV/Processing/Image.hpp"

namespace Arcv {

namespace Image {

namespace {

const uint8_t PNG_HEADER_SIZE = 8;

const std::string extractFileExt(const std::string& fileName) {
  return (fileName.substr(fileName.find_last_of('.') + 1));
}

bool validatePng(std::istream& file) {
  std::array<png_byte, PNG_HEADER_SIZE> header;
  file.read(reinterpret_cast<char*>(header.data()), PNG_HEADER_SIZE);

  return (png_sig_cmp(header.data(), 0, PNG_HEADER_SIZE) == 0);
}

Matrix<> readJpeg(std::ifstream& file) {
  throw std::runtime_error("Error: JPEG reading not yet implemented");
}

Matrix<> readPng(std::ifstream& file) {
  if (!file.good() || !validatePng(file))
    throw std::runtime_error("Error: Not a valid PNG");

  png_structp readStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!readStruct)
    throw std::runtime_error("Error: Couldn't initialize PNG read struct");

  png_infop infoStruct = png_create_info_struct(readStruct);
  if (!infoStruct)
    throw std::runtime_error("Error: Couldn't initialize PNG info struct");

  png_set_read_fn(readStruct, &file, [] (png_structp pngReadPtr, png_bytep data, png_size_t length) {
    png_voidp inPtr = png_get_io_ptr(pngReadPtr);
    static_cast<std::istream*>(inPtr)->read(reinterpret_cast<char*>(data), length);
  });

  // Setting the amount signature bytes we've already read
  png_set_sig_bytes(readStruct, PNG_HEADER_SIZE);

  png_read_info(readStruct, infoStruct);

  uint32_t width = png_get_image_width(readStruct, infoStruct);
  uint32_t height = png_get_image_height(readStruct, infoStruct);
  uint8_t bitDepth = png_get_bit_depth(readStruct, infoStruct);
  uint8_t channels = png_get_channels(readStruct, infoStruct);
  uint8_t colorType = png_get_color_type(readStruct, infoStruct);
  Colorspace colorspace;

  switch (colorType) {
    case PNG_COLOR_TYPE_GRAY:
      if (bitDepth < 8)
        png_set_expand_gray_1_2_4_to_8(readStruct);

      colorspace = ARCV_COLORSPACE_GRAY;
      bitDepth = 8;
      break;

    case PNG_COLOR_TYPE_GRAY_ALPHA:
      colorspace = ARCV_COLORSPACE_GRAY_ALPHA;
      break;

    case PNG_COLOR_TYPE_PALETTE:
      png_set_palette_to_rgb(readStruct);
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

  png_set_scale_16(readStruct);

  // Adding full alpha channel to the image if it possesses transparency
  if (png_get_valid(readStruct, infoStruct, static_cast<png_uint_32>(PNG_INFO_tRNS))) {
    png_set_tRNS_to_alpha(readStruct);
    colorspace = ARCV_COLORSPACE_RGBA;
    ++channels;
  }

  png_read_update_info(readStruct, infoStruct);

  Matrix<uint8_t> mat(width, height, channels, bitDepth, colorspace);

  std::vector<png_bytep> rowPtrs(height);

  // Mapping row's elements to data's
  for (unsigned int i = 0; i < height; ++i)
    rowPtrs[i] = &mat.getData()[width * channels * i];

  png_read_image(readStruct, rowPtrs.data());
  png_read_end(readStruct, infoStruct);
  png_destroy_read_struct(&readStruct, nullptr, &infoStruct);

  return Matrix<>(mat);
}

Matrix<> readTga(std::ifstream& file) {
  throw std::runtime_error("Error: TGA reading not yet implemented");
}

Matrix<> readBmp(std::ifstream& file) {
  throw std::runtime_error("Error: BMP reading not yet implemented");
}

Matrix<> readBpg(std::ifstream& file) {
  throw std::runtime_error("Error: BPG reading not yet implemented");
}

void writeJpeg(const Matrix<>& mat, std::ofstream& file) {
  throw std::runtime_error("Error: JPEG output not yet implemented");
}

void writePng(const Matrix<>& mat, std::ofstream& file) {
  const Matrix<uint8_t> matToWrite(mat);

  png_structp writeStruct = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!writeStruct)
    throw std::runtime_error("Error: Couldn't initialize PNG write struct");

  png_infop infoStruct = png_create_info_struct(writeStruct);
  if (!infoStruct)
    throw std::runtime_error("Error: Couldn't initialize PNG info struct");

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

  png_set_compression_level(writeStruct, 6);

  if (matToWrite.getChannelCount() * matToWrite.getImgBitDepth() >= 16) {
    png_set_compression_strategy(writeStruct, Z_FILTERED);
    png_set_filter(writeStruct, 0, PNG_FILTER_NONE | PNG_FILTER_SUB | PNG_FILTER_PAETH);
  } else {
    png_set_compression_strategy(writeStruct, Z_DEFAULT_STRATEGY);
  }

  png_set_IHDR(writeStruct,
               infoStruct,
               static_cast<png_uint_32>(matToWrite.getWidth()),
               static_cast<png_uint_32>(matToWrite.getHeight()),
               matToWrite.getImgBitDepth(),
               colorType,
               PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_BASE,
               PNG_FILTER_TYPE_BASE);

  png_set_write_fn(writeStruct, &file, [] (png_structp pngWritePtr, png_bytep data, png_size_t length) {
    png_voidp outPtr = png_get_io_ptr(pngWritePtr);
    static_cast<std::ostream*>(outPtr)->write(reinterpret_cast<const char*>(data), length);
  }, nullptr);
  png_write_info(writeStruct, infoStruct);

  for (std::size_t i = 0; i < matToWrite.getHeight(); ++i)
    png_write_row(writeStruct, &matToWrite.getData()[matToWrite.getWidth() * matToWrite.getChannelCount() * i]);

  png_write_end(writeStruct, infoStruct);
  png_destroy_write_struct(&writeStruct, &infoStruct);
}

void writeTga(const Matrix<>& mat, std::ofstream& file) {
  throw std::runtime_error("Error: TGA output not yet implemented");
}

void writeBmp(const Matrix<>& mat, std::ofstream& file) {
  throw std::runtime_error("Error: BMP output not yet implemented");
}

void writeBpg(const Matrix<>& mat, std::ofstream& file) {
  throw std::runtime_error("Error: BPG output not yet implemented");
}

} // namespace

Matrix<> read(const std::string& fileName) {
  std::ifstream file(fileName, std::ios_base::in | std::ios_base::binary);

  if (file) {
    const std::string format = extractFileExt(fileName);

    if (format == "jpg" || format == "jpeg" || format == "JPG" || format == "JPEG")
      return readJpeg(file);
    else if (format == "png" || format == "PNG")
      return readPng(file);
    else if (format == "tga" || format == "TGA")
      return readTga(file);
    else if (format == "bmp" || format == "BMP")
      return readBmp(file);
    else if (format == "bpg" || format == "BPG")
      return readBpg(file);
    else
      throw std::runtime_error("Error: '" + format + "' format is not supported");
  } else {
    throw std::runtime_error("Error: Couldn't open the file '" + fileName + "'");
  }
}

void write(const Matrix<>& mat, const std::string& fileName) {
  std::ofstream file(fileName, std::ios_base::out | std::ios_base::binary);
  const std::string format = extractFileExt(fileName);

  if (format == "jpg" || format == "jpeg" || format == "JPG" || format == "JPEG")
    writeJpeg(mat, file);
  else if (format == "png" || format == "PNG")
    writePng(mat, file);
  else if (format == "tga" || format == "TGA")
    writeTga(mat, file);
  else if (format == "bmp" || format == "BMP")
    writeBmp(mat, file);
  else if (format == "bpg" || format == "BPG")
    writeBpg(mat, file);
  else
    throw std::runtime_error("Error: '" + format + "' format is not supported");
}

} // namespace Image

} // namespace Arcv
