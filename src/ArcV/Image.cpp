#include "Image.hpp"

bool validate(std::istream& file) {
  png_byte header[PNG_HEADER_SIZE];

  // Reading the 8 bytes of the header from the stream
  file.read((char*)header, PNG_HEADER_SIZE);

  if (!file.good())
    return false;

  // Checking if the header's correct
  return (png_sig_cmp(header, 0, PNG_HEADER_SIZE) == 0);
}

void readPng(png_structp pngPtr, png_bytep data, png_size_t length) {
  // Getting IO pointer from read struct
  png_voidp ioPtr = png_get_io_ptr(pngPtr);
  // Casting pointer to std::istream* and reading <length> bytes into <data>
  static_cast<std::istream*>(ioPtr)->read((char*)data, length);
}

template <>
void Image<PNG>::read(const std::string& fileName) {
  std::ifstream file(fileName);

  if (!validate(file)) {
    std::cerr << "Error: " << fileName << " is not a valid PNG." << std::endl;
    return;
  }

  // Creating PNG read structure and checking whether it has been created correctly
  png_structp pngReadStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!pngReadStruct) {
    std::cerr << "Error: Couldn't initialize PNG read struct." << std::endl;
    return;
  }

  // Creating PNG info structure and checking whether it has been created correctly
  // If not, deleting read structure
  png_infop pngInfoStruct = png_create_info_struct(pngReadStruct);
  if (!pngInfoStruct) {
    std::cerr << "Error: Couldn't initialize PNG info struct." << std::endl;
    png_destroy_read_struct(&pngReadStruct, static_cast<png_infopp>(0), static_cast<png_infopp>(0));
    return;
  }

  png_set_read_fn(pngReadStruct, static_cast<png_voidp>(&file), readPng);

  // Setting the amount signature bytes we've already read
  png_set_sig_bytes(pngReadStruct, PNG_HEADER_SIZE);

  // Calling png_read_info with our pngReadStruct as image handle, and pngInfoStruct to receive file infos
  png_read_info(pngReadStruct, pngInfoStruct);

  // Getting image's width & height
  png_uint_32 imgWidth = png_get_image_width(pngReadStruct, pngInfoStruct);
  png_uint_32 imgHeight = png_get_image_height(pngReadStruct, pngInfoStruct);

  // Getting bits per channel (not per pixel)
  png_uint_32 bitDepth = png_get_bit_depth(pngReadStruct, pngInfoStruct);
  // Getting number of channels
  png_uint_32 channels = png_get_channels(pngReadStruct, pngInfoStruct);
  // Getting color type (RGB, RGBA, luminance, alpha, palette, etc)
  png_uint_32 colorType = png_get_color_type(pngReadStruct, pngInfoStruct);

  // Refining color type (if colored or grayscale)
  switch (colorType) {
    case PNG_COLOR_TYPE_PALETTE:
      png_set_palette_to_rgb(pngReadStruct);

      // If RBG image, setting channels to 3
      channels = 3;
      break;

    case PNG_COLOR_TYPE_GRAY:
      if (bitDepth < 8)
        png_set_expand_gray_1_2_4_to_8(pngReadStruct);

      // Updating bitdepth info
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

  // Defining an array to contain image's rows of pixels
  png_bytep rowPtrs[imgHeight];

  // Defining an array to contain image's pixels
  data = new char[imgWidth * imgHeight * bitDepth * channels / 8];

  const unsigned int rowLength = imgWidth * bitDepth * channels / 8;

  // Adding every pixel into previously allocated rows
  for (unsigned int i = 0; i < imgHeight; i++) {
    //Set the pointer to the data pointer + i times the row stride.
    //Notice that the row order is reversed with q.
    //This is how at least OpenGL expects it,
    //and how many other image loaders present the data.
    rowPtrs[i] = (png_bytep)data + ((imgHeight - i - 1) * rowLength);
  }

  //And here it is! The actuall reading of the image!
  //Read the imagedata and write it to the adresses pointed to
  //by rowptrs (in other words: our image databuffer)
  png_read_image(pngReadStruct, rowPtrs);

  png_destroy_read_struct(&pngReadStruct, static_cast<png_infopp>(0), static_cast<png_infopp>(0));
}

template <>
void Image<JPEG>::write(const std::string& fileName) {}

template <>
void Image<PNG>::write(const std::string& fileName) {}
