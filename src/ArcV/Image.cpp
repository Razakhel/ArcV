#include <png++/image.hpp>

#include "Image.hpp"

void writeJpegImage(std::string fileName) {}

void writePngImage(std::string fileName) {
  png::image<png::rgb_pixel> image(128, 128);

  for (png::uint_32 y = 0; y < image.get_height(); ++y) {
    for (png::uint_32 x = 0; x < image.get_width(); ++x) {
      image[x][y] = png::rgb_pixel(x, y, x + y);
    }
  }

  image.write(fileName);
}

void Image::writeImage(const std::string& fileName, Image::Type type) {
  switch (type) {
    case JPEG:
      writeJpegImage(fileName);
      break;
    case PNG:
      writePngImage(fileName);
      break;
    default:
      break;
  }
}
