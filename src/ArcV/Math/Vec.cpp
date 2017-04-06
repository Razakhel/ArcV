#include <cassert>

#include "ArcV/Math/Vec.hpp"

namespace Arcv {

const float Vec::dot(Vec& vec) const {
  assert(("Error: Vectors aren't the same size", data.size() == vec.getData().size()));

  float res = 0.f;
  for (unsigned int i = 0; i < data.size(); ++i)
    res += data[i] * vec.getData()[i];
  return res;
}

Vec& Vec::operator+=(Vec& vec) {
  assert(("Error: Vectors aren't the same size", data.size() == vec.getData().size()));

  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] += vec.getData()[i];
  return *this;
}

Vec& Vec::operator+=(const float& val) {
  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] += val;
  return *this;
}

Vec& Vec::operator-=(Vec& vec) {
  assert(("Error: Vectors aren't the same size", data.size() == vec.getData().size()));

  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] -= vec.getData()[i];
  return *this;
}

Vec& Vec::operator-=(const float& val) {
  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] -= val;
  return *this;
}

Vec& Vec::operator*=(Vec& vec) {
  assert(("Error: Vectors aren't the same size", data.size() == vec.getData().size()));

  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] *= vec.getData()[i];
  return *this;
}

Vec& Vec::operator*=(const float& val) {
  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] *= val;
  return *this;
}

Vec& Vec::operator/=(Vec& vec) {
  assert(("Error: Vectors aren't the same size", data.size() == vec.getData().size()));

  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] /= vec.getData()[i];
  return *this;
}

Vec& Vec::operator/=(const float& val) {
  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] /= val;
  return *this;
}

} // namespace Arcv
