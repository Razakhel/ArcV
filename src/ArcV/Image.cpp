#include "Image.hpp"

template <Type T>
Image<T>::Image(const std::string& fileName) {
  read(fileName);
}

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
  //TODO: load image
}

template <>
void Image<JPEG>::write(const std::string& fileName) {}

template <>
void Image<PNG>::write(const std::string& fileName) {}
