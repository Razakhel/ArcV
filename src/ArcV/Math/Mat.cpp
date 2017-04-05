#include <cassert>

#include "Mat.hpp"

namespace Arcv {

Mat& Mat::operator+(Mat& mat) {
  assert(("Error: Matrices aren't the same size", data.size() == mat.getData().size()));

  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] += mat.getData()[i];
  return *this;
}

Mat& Mat::operator+(const float& val) {
  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] += val;
  return *this;
}

Mat& Mat::operator-(Mat& mat) {
  assert(("Error: Matrices aren't the same size", data.size() == mat.getData().size()));

  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] -= mat.getData()[i];
  return *this;
}

Mat& Mat::operator-(const float& val) {
  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] -= val;
  return *this;
}

Mat& Mat::operator*=(Mat& mat) {
  assert(("Error: Matrices aren't the same size", data.size() == mat.getData().size()));

  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] *= mat.getData()[i];
  return *this;
}

Mat& Mat::operator*=(const float& val) {
  for (unsigned int i = 0; i < data.size(); ++i)
    data[i] *= val;
  return *this;
}

} // namespace Arcv
