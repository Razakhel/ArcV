#pragma once

#ifndef ARCV_VEC_HPP
#define ARCV_VEC_HPP

#include <vector>

namespace Arcv {

class Vec {
public:
  Vec(const unsigned int size) : data(size) {}
  //Vec(const Vec& vec);

  std::vector<float>& getData() { return data; }

  const float dot(Vec& vec) const;

  Vec& operator+(Vec& vec);
  Vec& operator+(const float& val);
  Vec& operator-(Vec& vec);
  Vec& operator-(const float& val);
  Vec& operator*(Vec& vec);
  Vec& operator*(const float& val);
  Vec& operator*=(Vec& vec) { return *this = *this * vec; }
  Vec& operator*=(const float& val) { return *this = *this * val; }
  float& operator[](const unsigned int index) { return data[index]; }

private:
  std::vector<float> data;
};

} // namespace Arcv

#endif // ARCV_VEC_HPP
