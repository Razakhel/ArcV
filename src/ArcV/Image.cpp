#include "Image.hpp"

namespace Arcv {

namespace {

bool validatePng(std::istream& file) {
  std::array<png_byte, PNG_HEADER_SIZE> header;

  file.read(reinterpret_cast<char*>(header.data()), PNG_HEADER_SIZE);

  if (!file.good())
    return false;

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

} // namespace

template <>
void Image<JPEG>::read(const std::string& fileName) {}

template <>
void Image<PNG>::read(const std::string& fileName) {
  std::ifstream file(fileName);
  assert(("Error: Not a valid PNG.", validatePng(file)));

  png_structp pngReadStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  assert(("Error: Couldn't initialize PNG read struct.", pngReadStruct));

  png_infop pngInfoStruct = png_create_info_struct(pngReadStruct);
  assert(("Error: Couldn't initialize PNG info struct.", pngInfoStruct));

  png_set_read_fn(pngReadStruct, &file, readPng);

  // Setting the amount signature bytes we've already read
  png_set_sig_bytes(pngReadStruct, PNG_HEADER_SIZE);

  png_read_info(pngReadStruct, pngInfoStruct);

  width = png_get_image_width(pngReadStruct, pngInfoStruct);
  height = png_get_image_height(pngReadStruct, pngInfoStruct);
  bitDepth = png_get_bit_depth(pngReadStruct, pngInfoStruct);
  channels = png_get_channels(pngReadStruct, pngInfoStruct);
  colorType = png_get_color_type(pngReadStruct, pngInfoStruct);

  // Refining color type (if colored or grayscale)
  switch (colorType) {
    case PNG_COLOR_TYPE_PALETTE:
      png_set_palette_to_rgb(pngReadStruct);

      channels = 3;
      break;

    case PNG_COLOR_TYPE_GRAY:
      if (bitDepth < 8)
        png_set_expand_gray_1_2_4_to_8(pngReadStruct);

      bitDepth = 8;
      break;

    default:
      break;
  }

  png_set_scale_16(pngReadStruct);

  // Adding full alpha channel to the image if it possesses transparency
  if (png_get_valid(pngReadStruct, pngInfoStruct, PNG_INFO_tRNS)) {
    png_set_tRNS_to_alpha(pngReadStruct);
    channels += 1;
  }

  png_read_update_info(pngReadStruct, pngInfoStruct);

  std::vector<png_bytep> rowPtrs(height * sizeof(png_bytepp));

  // Defining an array to contain image's pixels
  data = std::make_unique<uint8_t[]>(width * height * channels);

  // Mapping row's elements to data's
  for (png_uint_32 i = 0; i < height; ++i) {
    rowPtrs[i] = &data.get()[width * channels * i];
  }

  png_read_image(pngReadStruct, rowPtrs.data());
  png_read_end(pngReadStruct, pngInfoStruct);
  png_destroy_read_struct(&pngReadStruct, static_cast<const png_infopp>(0), static_cast<const png_infopp>(0));
}

template <>
void Image<JPEG>::write(const std::string& fileName) {}

template <>
void Image<PNG>::write(const std::string& fileName) {
  std::ofstream file(fileName);

  png_structp pngWriteStruct = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  assert(("Error: Couldn't initialize PNG write struct.", pngWriteStruct));

  png_infop pngInfoStruct = png_create_info_struct(pngWriteStruct);
  assert(("Error: Couldn't initialize PNG info struct.", pngInfoStruct));

  switch (channels) {
    case 1:
      colorType = PNG_COLOR_TYPE_GRAY;
      break;

    case 2:
      colorType = PNG_COLOR_TYPE_GRAY_ALPHA;
      break;

    case 3:
      colorType = PNG_COLOR_TYPE_RGB;
      break;

    case 4:
      colorType = PNG_COLOR_TYPE_RGBA;
      break;

    default:
      break;
  }

  png_set_compression_level(pngWriteStruct, 6);

  if (channels * bitDepth >= 16) {
    png_set_compression_strategy(pngWriteStruct, Z_FILTERED);
    png_set_filter(pngWriteStruct, 0, PNG_FILTER_NONE | PNG_FILTER_SUB | PNG_FILTER_PAETH);
  } else {
    png_set_compression_strategy(pngWriteStruct, Z_DEFAULT_STRATEGY);
  }

  png_set_IHDR(pngWriteStruct,
               pngInfoStruct,
               width,
               height,
               static_cast<uint32_t>(bitDepth),
               static_cast<uint32_t>(colorType),
               PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_BASE,
               PNG_FILTER_TYPE_BASE);

  png_set_write_fn(pngWriteStruct, &file, writePng, nullptr);
  png_write_info(pngWriteStruct, pngInfoStruct);

  for (png_uint_32 i = 0; i < height; ++i) {
    png_write_row(pngWriteStruct, &data.get()[width * channels * i]);
  }

  png_write_end(pngWriteStruct, pngInfoStruct);
  png_destroy_write_struct(&pngWriteStruct, &pngInfoStruct);
}

} // namespace Arcv