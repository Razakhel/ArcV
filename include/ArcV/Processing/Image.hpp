#pragma once

#ifndef ARCV_IMAGE_HPP
#define ARCV_IMAGE_HPP

#include "Mat.hpp"

namespace Arcv {

const unsigned int PNG_HEADER_SIZE = 8;
enum ImageType { JPEG = 0, PNG };

template <ImageType T>
class Image {
public:
  static Mat read(const std::string& fileName);
  static void write(const Mat& mat, const std::string& fileName);
};

} // namespace Arcv

#endif // ARCV_IMAGE_HPP
