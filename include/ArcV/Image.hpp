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

  Image<T>() {}
  Image<T>(const std::string& fileName);

  void read(const std::string& fileName);
  void write(const std::string& fileName);
};

#endif