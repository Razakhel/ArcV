#include "Image.hpp"

Image::Image(const std::string& fileName) {
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

void Image::read(const std::string& fileName) {
  std::ifstream file(fileName);
  if (!validate(file)) {
    std::cerr << "Error: " << fileName << " is not a valid PNG." << std::endl;
    return;
  }

  // Creating PNG read structure and checking whether it has been created correctly
  png_structp pngRead = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!pngRead) {
    std::cerr << "Error: Couldn't initialize PNG read struct." << std::endl;
    return;
  }

  // Creating PNG info structure and checking whether it has been created correctly
  // If not, deleting read structure
  png_infop pngInfo = png_create_info_struct(pngRead);
  if (!pngInfo) {
    std::cerr << "Error: Couldn't initialize PNG info struct." << std::endl;
    png_destroy_read_struct(&pngRead, static_cast<png_infopp>(0), static_cast<png_infopp>(0));
    return;
  }

  //TODO: load image
}

void writeJpeg(const std::string& fileName) {}

void writePng(const std::string& fileName) {}

void Image::write(const std::string& fileName, const Image::Type type) const {
  switch (type) {
    case JPEG:
      writeJpeg(fileName);
      break;
    case PNG:
      writePng(fileName);
      break;
    default:
      break;
  }
}
