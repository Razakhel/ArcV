#pragma once

#ifndef ARCV_MAT_HPP
#define ARCV_MAT_HPP

#include <vector>

namespace Arcv {

class Mat {
public:
  Mat() {}
  Mat(const unsigned int width, const unsigned int height) : width{ width }, height{ height }, data(width * height) {}

  const unsigned int getWidth() const { return width; }
  const unsigned int getHeight() const { return height; }
  const std::vector<float>& getData() const { return data; }

private:
  unsigned int width;
  unsigned int height;
  std::vector<float> data;
};

} // namespace Arcv

#endif // ARCV_MAT_HPP
