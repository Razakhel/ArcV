#pragma once

#ifndef ARCV_MATRIX_HPP
#define ARCV_MATRIX_HPP

#include <cmath>
#include <vector>
#include <cstdint>

enum Colorspace { ARCV_COLORSPACE_GRAY = 0,
                  ARCV_COLORSPACE_RGB,
                  ARCV_COLORSPACE_HSV,
                  ARCV_COLORSPACE_GRAY_ALPHA,
                  ARCV_COLORSPACE_RGBA };

namespace Arcv {

template <typename T = float>
class Matrix {
public:
  Matrix() = default;

  Matrix(std::size_t width,
         std::size_t height)
    : width{ width }, height{ height }, data(width * height) {}

  Matrix(std::size_t width,
         std::size_t height,
         uint8_t channels,
         uint8_t bitDepth,
         Colorspace colorspace)
    : width{ width },
      height{ height },
      channelCount{ channels },
      imgBitDepth{ bitDepth },
      colorspace{ colorspace },
      data(width * height * channels) {}

  template <typename TI> Matrix(const Matrix<TI>& mat);

  Matrix(std::initializer_list<std::initializer_list<T>> list);

  std::size_t getWidth() const { return width; }
  std::size_t getHeight() const { return height; }
  uint8_t getImgBitDepth() const { return imgBitDepth; }
  uint8_t getChannelCount() const { return channelCount; }
  Colorspace getColorspace() const { return colorspace; }
  const std::vector<T>& getData() const { return data; }
  std::vector<T>& getData() { return data; }

  void setChannelCount(uint8_t channelCount) { this->channelCount = channelCount; }
  void setColorspace(Colorspace colorspace) { this->colorspace = colorspace; }

  Matrix convolve(const Matrix<float>& convMat) const;
  std::pair<T, T> determineBoundaries() const;
  std::vector<T> computeAverageValues() const;
  std::vector<T> computeStandardDeviations() const;

  Matrix operator+(Matrix mat);
  Matrix operator+(float val);
  Matrix operator-(Matrix mat);
  Matrix operator-(float val);
  Matrix operator*(Matrix mat);
  Matrix operator*(float val);
  Matrix operator/(Matrix mat);
  Matrix operator/(float val);
  Matrix& operator+=(const Matrix& mat);
  Matrix& operator+=(float val);
  Matrix& operator-=(const Matrix& mat);
  Matrix& operator-=(float val);
  Matrix& operator*=(const Matrix& mat);
  Matrix& operator*=(float val);
  Matrix& operator/=(const Matrix& mat);
  Matrix& operator/=(float val);
  const T& operator()(std::size_t widthIndex, std::size_t heightIndex) const { return data[heightIndex * width + widthIndex]; }
  T& operator()(std::size_t widthIndex, std::size_t heightIndex) { return data[heightIndex * width + widthIndex]; }
  const T& operator[](std::size_t index) const { return data[index]; }
  T& operator[](std::size_t index) { return data[index]; } // Implement Pixel class to return an instance?

private:
  std::size_t width, height;
  uint8_t channelCount = 1, imgBitDepth = 8;    // TODO: channels, depth & colorspace have nothing to do with general matrices
  Colorspace colorspace = ARCV_COLORSPACE_GRAY;
  std::vector<T> data;
};

template <> Matrix<> Matrix<>::convolve(const Matrix<float>& convMat) const;

using Mat = Matrix<>;

} // namespace Arcv

#include "ArcV/Math/Matrix.inl"

#endif // ARCV_MATRIX_HPP
