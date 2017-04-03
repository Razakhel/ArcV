#include "Vec.hpp"

namespace Arcv {

const float Vec::dot(Vec& vec) const {
  float res = 0.f;
  for (unsigned int i = 0; i < data.size(); ++i)
    res += data[i] * vec.getData()[i];
  return res;
}

Vec& Vec::operator+(Vec& vec) {
  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] += vec.getData()[i];
  return *this;
}

Vec& Vec::operator+(const float& val) {
  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] += val;
  return *this;
}

Vec& Vec::operator-(Vec& vec) {
  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] -= vec.getData()[i];
  return *this;
}

Vec& Vec::operator-(const float& val) {
  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] -= val;
  return *this;
}

Vec& Vec::operator*=(Vec& vec) {
  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] *= vec.getData()[i];
  return *this;
}

Vec& Vec::operator*=(const float& val) {
  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] *= val;
  return *this;
}

} // namespace Arcv
