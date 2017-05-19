#include <limits>
#include <cassert>
#include <algorithm>

namespace Arcv {

template <typename T>
template <typename TI>
Matrix<T>::Matrix(const Matrix<TI>& mat)
  : width{ mat.getWidth() },
    height{ mat.getHeight() },
    channels{ mat.getChannels() },
    imgBitDepth{ mat.getImgBitDepth() },
    colorspace{ mat.getColorspace() },
    data(mat.getWidth() * mat.getHeight() * mat.getChannels()) {
  if (std::numeric_limits<TI>::max() > std::numeric_limits<T>::max()) {
    for (unsigned int eltIndex = 0; eltIndex < mat.getData().size(); ++eltIndex) {
      data[eltIndex] = std::min(std::numeric_limits<TI>::max(), std::max(static_cast<TI>(0), mat.getData()[eltIndex]));
    }
  } else {
    std::copy(mat.getData().begin(), mat.getData().end(), data.begin());
  }
}

template <typename T>
Matrix<T>::Matrix(const std::initializer_list<std::initializer_list<T>>& list)
  : height{ static_cast<unsigned int>(list.size()) },
    width{ static_cast<unsigned int>(list.begin()->size()) } {
  data.resize(list.size() * list.begin()->size());

  auto row = list.begin();
  for (unsigned int heightIndex = 0; heightIndex < list.size(); ++heightIndex, ++row) {
    auto element = row->begin();

    for (unsigned int widthIndex = 0; widthIndex < list.begin()->size(); ++widthIndex, ++element)
      data[heightIndex * list.begin()->size() + widthIndex] = *element;
  }
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix& mat) {
  assert(("Error: Matrices aren't the same size", data.size() == mat.getData().size()));

  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] += mat.getData()[i];
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(float val) {
  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] += val;
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix& mat) {
  assert(("Error: Matrices aren't the same size", data.size() == mat.getData().size()));

  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] -= mat.getData()[i];
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator-=(float val) {
  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] -= val;
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix& mat) {
  assert(("Error: Matrices aren't the same size", data.size() == mat.getData().size()));

  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] *= mat.getData()[i];
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(float val) {
  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] *= val;
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator/=(const Matrix& mat) {
  assert(("Error: Matrices aren't the same size", data.size() == mat.getData().size()));

  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] /= mat.getData()[i];
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator/=(float val) {
  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] /= val;
  return *this;
}

} // namespace Arcv
