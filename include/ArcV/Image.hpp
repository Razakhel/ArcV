#pragma once

#include <string>

class Image {
public:
  enum Type { JPEG, PNG };

  void writeImage(const std::string& fileName, Image::Type type);
};
