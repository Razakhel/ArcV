#include <limits>
#include <cassert>
#include <numeric>
#include <algorithm>
#include <functional>

namespace Arcv {

template <typename T>
template <typename TI>
Matrix<T>::Matrix(const Matrix<TI>& mat) : Matrix(mat.getWidth(),
                                                  mat.getHeight(),
                                                  mat.getChannelCount(),
                                                  mat.getImgBitDepth(),
                                                  mat.getColorspace()) {
  if (std::numeric_limits<TI>::max() > std::numeric_limits<T>::max()) {
    for (std::size_t eltIndex = 0; eltIndex < mat.getData().size(); ++eltIndex) {
      data[eltIndex] = std::max(static_cast<TI>(std::numeric_limits<T>::min()),
                                std::min(static_cast<TI>(std::numeric_limits<T>::max()),
                                         static_cast<TI>(mat.getData()[eltIndex])));
    }
  } else {
    std::copy(mat.getData().cbegin(), mat.getData().cend(), data.begin());
  }
}

template <typename T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> list)
  : Matrix(static_cast<unsigned int>(list.begin()->size()),
           static_cast<unsigned int>(list.size())) {
  auto row = list.begin();

  for (std::size_t heightIndex = 0; heightIndex < list.size(); ++heightIndex, ++row) {
    auto element = row->begin();

    for (std::size_t widthIndex = 0; widthIndex < list.begin()->size(); ++widthIndex, ++element)
      data[heightIndex * list.begin()->size() + widthIndex] = *element;
  }
}

template <typename T>
std::pair<T, T> Matrix<T>::determineBoundaries() const {
  std::pair<T, T> bounds;

  for (const auto& it : data) {
    if (it < bounds.first)
      bounds.first = it;
    else if (it > bounds.second)
      bounds.second = it;
  }

  return bounds;
}

template <typename T>
std::vector<T> Matrix<T>::computeAverageValues() const {
  std::vector<T> res(channelCount);
  std::vector<long double> values(channelCount);

  for (std::size_t heightIndex = 0; heightIndex < height; ++heightIndex) {
    for (std::size_t widthIndex = 0; widthIndex < width; ++widthIndex) {
      const std::size_t matIndex = (heightIndex * width + widthIndex) * channelCount;

      for (uint8_t chan = 0; chan < channelCount; ++chan)
        values[chan] += data[matIndex + chan];
    }
  }

  const std::size_t nbPixels = width * height;
  std::transform(values.cbegin(), values.cend(), res.begin(), std::bind2nd(std::divides<T>(), nbPixels));

  return res;
}

template <typename T>
std::vector<T> Matrix<T>::computeStandardDeviations() const {
  const std::vector<T> avgValues = computeAverageValues();
  std::vector<T> res(channelCount);
  std::vector<long double> values(channelCount);

  for (std::size_t heightIndex = 0; heightIndex < height; ++heightIndex) {
    for (std::size_t widthIndex = 0; widthIndex < width; ++widthIndex) {
      const std::size_t matIndex = (heightIndex * width + widthIndex) * channelCount;

      for (uint8_t chan = 0; chan < channelCount; ++chan) {
        const std::size_t value = data[matIndex + chan] - avgValues[chan];

        values[chan] += value * value;
      }
    }
  }

  const std::size_t nbPixels = width * height;
  for (std::size_t i = 0; i < res.size(); ++i)
    res[i] = std::sqrt(values[i] / nbPixels);

  return res;
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
