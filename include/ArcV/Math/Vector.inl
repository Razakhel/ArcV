#include <cassert>

namespace Arcv {

template <typename T>
Vector<T>::Vector(std::initializer_list<T> list) : data(list.size()) {
  auto element = list.begin();
  for (std::size_t i = 0; i < list.size(); ++i, ++element)
    data[i] = *element;
}

template <typename T>
T Vector<T>::dot(const Vector& vec) const {
  assert(("Error: Vectors aren't the same size", data.size() == vec.getData().size()));

  float res = 0.f;
  for (std::size_t i = 0; i < data.size(); ++i)
    res += data[i] * vec.getData()[i];
  return res;
}

template <typename T>
Vector<T> Vector<T>::normalize() const {
  Vector<T> res = *this;

  res /= std::sqrt(dot(*this));
  return res;
}

template <typename T>
Vector<T> Vector<T>::operator+(Vector vec) {
  assert(("Error: Vectors aren't the same size", data.size() == vec.getData().size()));

  vec += *this;
  return vec;
}

template <typename T>
Vector<T> Vector<T>::operator+(float val) {
  Vector<T> res = *this;
  res += val;
  return res;
}

template <typename T>
Vector<T> Vector<T>::operator-(Vector vec) {
  assert(("Error: Vectors aren't the same size", data.size() == vec.getData().size()));

  vec -= *this;
  return vec;
}

template <typename T>
Vector<T> Vector<T>::operator-(float val) {
  Vector<T> res = *this;
  res -= val;
  return res;
}

template <typename T>
Vector<T> Vector<T>::operator*(Vector vec) {
  assert(("Error: Vectors aren't the same size", data.size() == vec.getData().size()));

  vec *= *this;
  return vec;
}

template <typename T>
Vector<T> Vector<T>::operator*(float val) {
  Vector<T> res = *this;
  res *= val;
  return res;
}

template <typename T>
Vector<T> Vector<T>::operator/(Vector vec) {
  assert(("Error: Vectors aren't the same size", data.size() == vec.getData().size()));

  vec /= *this;
  return vec;
}

template <typename T>
Vector<T> Vector<T>::operator/(float val) {
  Vector<T> res = *this;
  res /= val;
  return res;
}

template <typename T>
Vector<T>& Vector<T>::operator+=(const Vector& vec) {
  assert(("Error: Vectors aren't the same size", data.size() == vec.getData().size()));

  for (std::size_t i = 0; i < data.size(); ++i)
    data[i] += vec.getData()[i];
  return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator+=(float val) {
  for (T& it : data)
    it += val;
  return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator-=(const Vector& vec) {
  assert(("Error: Vectors aren't the same size", data.size() == vec.getData().size()));

  for (std::size_t i = 0; i < data.size(); ++i)
    data[i] -= vec.getData()[i];
  return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator-=(float val) {
  for (T& it : data)
    it -= val;
  return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator*=(const Vector& vec) {
  assert(("Error: Vectors aren't the same size", data.size() == vec.getData().size()));

  for (std::size_t i = 0; i < data.size(); ++i)
    data[i] *= vec.getData()[i];
  return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator*=(float val) {
  for (T& it : data)
    it *= val;
  return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator/=(const Vector& vec) {
  assert(("Error: Vectors aren't the same size", data.size() == vec.getData().size()));

  for (std::size_t i = 0; i < data.size(); ++i)
    data[i] /= vec.getData()[i];
  return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator/=(float val) {
  for (T& it : data)
    it /= val;
  return *this;
}

} // namespace Arcv
