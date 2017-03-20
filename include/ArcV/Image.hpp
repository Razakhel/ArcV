#pragma once

#ifndef ARCV_IMAGE_HPP
#define ARCV_IMAGE_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <png.h>
#include <zlib.h>

const unsigned int PNG_HEADER_SIZE = 8;
enum Type { JPEG = 0, PNG };

template <Type T>
class Image {
public:
  Image() {}
  Image(const std::string& fileName);

  void read(const std::string& fileName);
  void write(const std::string& fileName);

  ~Image();

private:
  char* data;
};

template <Type T>
Image<T>::Image(const std::string& fileName) {
  read(fileName);
}

template <Type T>
Image<T>::~Image() {
  delete[] data;
}

#endif