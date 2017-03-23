#pragma once

#ifndef ARCV_IMAGE_HPP
#define ARCV_IMAGE_HPP

#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <iostream>
#include <png.h>
#include <zlib.h>

namespace Arcv {

const unsigned int PNG_HEADER_SIZE = 8;
enum ImageType { JPEG = 0, PNG };

template <ImageType T>
class Image {
public:
  Image() {}

  Image(const std::string& fileName);

  const uint8_t* getData() const { return data.get(); }
  const png_uint_32 getWidth() const { return width; }
  const png_uint_32 getHeight() const { return height; }
  const png_uint_32 getBitDepth() const { return bitDepth; }

  void read(const std::string& fileName);
  void write(const std::string& fileName);

private:
  std::unique_ptr<uint8_t[]> data;
  png_uint_32 width, height, bitDepth;
};

template <ImageType T>
Image<T>::Image(const std::string& fileName) {
  read(fileName);
}

} // namespace Arcv

#endif // ARCV_IMAGE_HPP