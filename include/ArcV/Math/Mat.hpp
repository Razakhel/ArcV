#pragma once

#ifndef ARCV_MAT_HPP
#define ARCV_MAT_HPP

#include <vector>

#include "Vec.hpp"

namespace Arcv {

enum Colorspace { GRAYSCALE = 0, RGB, RGBA, HSV };

class Mat {
public:
  Mat(const unsigned int width, const unsigned int height, const unsigned int channels)
    : width{ width }, height{ height }, data(width * height * channels) {}

  const unsigned int getWidth() const { return width; }
  const unsigned int getHeight() const { return height; }
  std::vector<float>& getData() { return data; }

  void changeColorspace();
  Mat& convolve();

  Mat& operator+(Mat& mat);
  Mat& operator+(const float& val);
  Mat& operator-(Mat& mat);
  Mat& operator-(const float& val);
  Mat& operator*(Mat& mat) { return *this *= mat; }
  Mat& operator*(const float& val) { return *this *= val; }
  Mat& operator*=(Mat& mat);
  Mat& operator*=(const float& val);
  float& operator[](const unsigned int index) { return data[index]; }

private:
  unsigned int width, height;
  std::vector<float> data;
};

} // namespace Arcv

#endif // ARCV_MAT_HPP
