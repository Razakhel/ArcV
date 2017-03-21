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

  void read(const std::string& fileName);
  void write(const std::string& fileName);

private:
  std::unique_ptr<char[]> data;
};

template <ImageType T>
Image<T>::Image(const std::string& fileName) {
  read(fileName);
}

} // namespace Arcv

#endif // ARCV_IMAGE_HPP