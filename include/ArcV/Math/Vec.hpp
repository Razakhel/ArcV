#pragma once

#ifndef ARCV_VEC_HPP
#define ARCV_VEC_HPP

#include <vector>

namespace Arcv {

class Vec {
public:
  Vec(const unsigned int size) : data(size) {}
  //Vec(const Vec& vec);

  const std::vector<float>& getData() const { return data; }
  std::vector<float>& getData() { return data; }

  const float dot(const Vec& vec) const;

  Vec& operator+(const Vec& vec) { return *this += vec; }
  Vec& operator+(float val) { return *this += val; }
  Vec& operator-(const Vec& vec) { return *this -= vec; }
  Vec& operator-(float val) { return *this -= val; }
  Vec& operator*(const Vec& vec) { return *this *= vec; }
  Vec& operator*(float val) { return *this *= val; }
  Vec& operator/(const Vec& vec) { return *this /= vec; }
  Vec& operator/(float val) { return *this /= val; }
  Vec& operator+=(const Vec& vec);
  Vec& operator+=(float val);
  Vec& operator-=(const Vec& vec);
  Vec& operator-=(float val);
  Vec& operator*=(const Vec& vec);
  Vec& operator*=(float val);
  Vec& operator/=(const Vec& vec);
  Vec& operator/=(float val);
  float& operator[](const unsigned int index) { return data[index]; }

private:
  std::vector<float> data;
};

} // namespace Arcv

#endif // ARCV_VEC_HPP
