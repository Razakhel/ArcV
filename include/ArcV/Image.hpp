#pragma once

#ifndef ARCV_IMAGE_HPP
#define ARCV_IMAGE_HPP

#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <png.h>
#include <zlib.h>

const unsigned int PNG_HEADER_SIZE = 8;
enum ImageType { JPEG = 0, PNG };

template <ImageType T>
class Image {
public:
  Image() {}
  Image(const std::string& fileName);

  void read(const std::string& fileName);
  void write(const std::string& fileName);

  friend std::ostream& operator<<(std::ostream& os, Vec3& vec) {
    return (os << "[ " << vec.x << ", " << vec.y << ", " << vec.z << " ]");
  }

private:
  std::unique_ptr<char[]> data;
};

template <ImageType T>
Image<T>::Image(const std::string& fileName) {
  read(fileName);
}

#endif // ARCV_IMAGE_HPP