#include "Image.hpp"

namespace Arcv {

namespace {

bool validatePng(std::istream& file) {
  png_byte header[PNG_HEADER_SIZE];

  file.read(reinterpret_cast<char*>(header), PNG_HEADER_SIZE);

  if (!file.good())
    return false;

  return (png_sig_cmp(header, 0, PNG_HEADER_SIZE) == 0);
}

void readPng(png_structp pngPtr, png_bytep data, png_size_t length) {
  png_voidp ioPtr = png_get_io_ptr(pngPtr);
  static_cast<std::istream*>(ioPtr)->read(reinterpret_cast<char*>(data), length);
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

  png_set_read_fn(pngReadStruct, static_cast<png_voidp>(&file), readPng);

  // Setting the amount signature bytes we've already read
  png_set_sig_bytes(pngReadStruct, PNG_HEADER_SIZE);

  png_read_info(pngReadStruct, pngInfoStruct);

  width = png_get_image_width(pngReadStruct, pngInfoStruct);
  height = png_get_image_height(pngReadStruct, pngInfoStruct);
  bitDepth = png_get_bit_depth(pngReadStruct, pngInfoStruct);
  png_uint_32 channels = png_get_channels(pngReadStruct, pngInfoStruct);
  png_uint_32 colorType = png_get_color_type(pngReadStruct, pngInfoStruct);

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

  // Adding full alpha channel to the image if it possesses transparency
  if (png_get_valid(pngReadStruct, pngInfoStruct, PNG_INFO_tRNS)) {
    png_set_tRNS_to_alpha(pngReadStruct);
    channels += 1;
  }

  std::vector<png_bytep> rowPtrs(height);

  // Defining an array to contain image's pixels
  data = std::make_unique<uint8_t[]>(width * height * bitDepth * channels / 8);

  const unsigned long int rowLength = width * bitDepth * channels / 8;

  for (unsigned int i = 0; i < height; ++i) {
    // Preparing the rows to handle image's data
    rowPtrs[i] = reinterpret_cast<png_bytep>(data[(height - i - 1) * rowLength]);
  }

  png_read_image(pngReadStruct, rowPtrs.data());
  png_destroy_read_struct(&pngReadStruct, static_cast<png_infopp>(0), static_cast<png_infopp>(0));
}

template <>
void Image<JPEG>::write(const std::string& fileName) {}

template <>
void Image<PNG>::write(const std::string& fileName) {}

} // namespace Arcv