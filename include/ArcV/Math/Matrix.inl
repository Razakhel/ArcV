#include <limits>
#include <cassert>
#include <algorithm>

namespace Arcv {

template <typename T>
template <typename TI>
Matrix<T>::Matrix(const Matrix<TI>& mat)
  : width{ mat.getWidth() },
    height{ mat.getHeight() },
    channelCount{ mat.getChannelCount() },
    imgBitDepth{ mat.getImgBitDepth() },
    colorspace{ mat.getColorspace() },
    data(mat.getWidth() * mat.getHeight() * mat.getChannelCount()) {
  if (std::numeric_limits<TI>::max() > std::numeric_limits<T>::max()) {
    for (std::size_t eltIndex = 0; eltIndex < mat.getData().size(); ++eltIndex) {
      data[eltIndex] = std::min(static_cast<TI>(std::abs(mat.getData()[eltIndex])), std::numeric_limits<TI>::max());
    }
  } else {
    std::copy(mat.getData().begin(), mat.getData().end(), data.begin());
  }
}

template <typename T>
Matrix<T>::Matrix(const std::initializer_list<const std::initializer_list<T>>& list)
  : width{ static_cast<unsigned int>(list.begin()->size()) },
    height{ static_cast<unsigned int>(list.size()) },
    data(list.size() * list.begin()->size()) {
  auto row = list.begin();

  for (std::size_t heightIndex = 0; heightIndex < list.size(); ++heightIndex, ++row) {
    auto element = row->begin();

    for (std::size_t widthIndex = 0; widthIndex < list.begin()->size(); ++widthIndex, ++element)
      data[heightIndex * list.begin()->size() + widthIndex] = *element;
  }
}

template <typename T>
Matrix<T> Matrix<T>::operator+(Matrix mat) {
  mat += *this;
  return mat;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(float val) {
  Matrix<T> res = *this;
  res += val;
  return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(Matrix mat) {
  mat -= *this;
  return mat;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(float val) {
  Matrix<T> res = *this;
  res -= val;
  return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(Matrix mat) {
  mat *= *this;
  return mat;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(float val) {
  Matrix<T> res = *this;
  res *= val;
  return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator/(Matrix mat) {
  mat /= *this;
  return mat;
}

template <typename T>
Matrix<T> Matrix<T>::operator/(float val) {
  Matrix<T> res = *this;
  res /= val;
  return res;
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix& mat) {
  assert(("Error: Matrices aren't the same size", data.size() == mat.getData().size()));

  for (std::size_t i = 0; i < data.size(); ++i)
    data[i] += mat.getData()[i];
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(float val) {
  for (T& it : data)
    it += val;
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix& mat) {
  assert(("Error: Matrices aren't the same size", data.size() == mat.getData().size()));

  for (std::size_t i = 0; i < data.size(); ++i)
    data[i] -= mat.getData()[i];
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator-=(float val) {
  for (T& it : data)
    it -= val;
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix& mat) {
  assert(("Error: Matrices aren't the same size", data.size() == mat.getData().size()));

  for (std::size_t i = 0; i < data.size(); ++i)
    data[i] *= mat.getData()[i];
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(float val) {
  for (T& it : data)
    it *= val;
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator/=(const Matrix& mat) {
  assert(("Error: Matrices aren't the same size", data.size() == mat.getData().size()));

  for (std::size_t i = 0; i < data.size(); ++i)
    data[i] /= mat.getData()[i];
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator/=(float val) {
  for (T& it : data)
    it /= val;
  return *this;
}

} // namespace Arcv
