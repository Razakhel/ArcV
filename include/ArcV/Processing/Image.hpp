#pragma once

#ifndef ARCV_IMAGE_HPP
#define ARCV_IMAGE_HPP

#include <memory>

namespace Arcv {

const unsigned int PNG_HEADER_SIZE = 8;
enum ImageType { JPEG = 0, PNG };

template <ImageType T>
class Image {
public:
  Image() {}
  Image(const std::string& fileName);

  const uint32_t getWidth() const { return width; }
  const uint32_t getHeight() const { return height; }
  const uint32_t getBitDepth() const { return bitDepth; }
  uint8_t* getData() const { return data.get(); }

  void read(const std::string& fileName);
  void write(const std::string& fileName);

private:
  uint32_t width, height, bitDepth, channels, colorType;
  std::unique_ptr<uint8_t[]> data;
};

template <ImageType T>
Image<T>::Image(const std::string& fileName) {
  read(fileName);
}

} // namespace Arcv

#endif // ARCV_IMAGE_HPP
