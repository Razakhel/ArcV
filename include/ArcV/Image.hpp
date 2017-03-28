#pragma once

#ifndef ARCV_IMAGE_HPP
#define ARCV_IMAGE_HPP

#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <cassert>
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

  uint8_t* getData() const { return data.get(); }
  const uint32_t getWidth() const { return width; }
  const uint32_t getHeight() const { return height; }
  const uint32_t getBitDepth() const { return bitDepth; }

  void read(const std::string& fileName);
  void write(const std::string& fileName);

private:
  std::unique_ptr<uint8_t[]> data;
  png_uint_32 width, height, bitDepth, channels, colorType;
};

template <ImageType T>
Image<T>::Image(const std::string& fileName) {
  read(fileName);
}

} // namespace Arcv

#endif // ARCV_IMAGE_HPP
