#pragma once

#ifndef ARCV_IMAGE_HPP
#define ARCV_IMAGE_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <png.h>

const unsigned int PNG_HEADER_SIZE = 8;

class Image {
public:
  enum Type { JPEG = 0, PNG };

  Image() {}
  Image(const std::string& fileName);

  void read(const std::string& fileName);
  void write(const std::string& fileName, Image::Type type) const;
};

#endif