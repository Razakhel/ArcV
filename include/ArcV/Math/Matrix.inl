#include <cassert>

namespace Arcv {

template <typename T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> list)
    : height{ list.size() }, width{ list.begin()->size() } {
  for (auto it = list.begin(); it != list.end(); ++it)
    for (auto it2 = it->begin(); it2 != it->end(); ++it2)
      data.push_back(*it2);
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(Matrix& mat) {
  assert(("Error: Matrices aren't the same size", data.size() == mat.getData().size()));

  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] += mat.getData()[i];
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(const float val) {
  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] += val;
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator-=(Matrix& mat) {
  assert(("Error: Matrices aren't the same size", data.size() == mat.getData().size()));

  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] -= mat.getData()[i];
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator-=(const float val) {
  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] -= val;
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(Matrix& mat) {
  assert(("Error: Matrices aren't the same size", data.size() == mat.getData().size()));

  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] *= mat.getData()[i];
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(const float val) {
  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] *= val;
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator/=(Matrix& mat) {
  assert(("Error: Matrices aren't the same size", data.size() == mat.getData().size()));

  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] /= mat.getData()[i];
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator/=(const float val) {
  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] /= val;
  return *this;
}

} // namespace Arcv
