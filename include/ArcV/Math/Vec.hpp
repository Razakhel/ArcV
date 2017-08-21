#pragma once

#ifndef ARCV_VEC_HPP
#define ARCV_VEC_HPP

#include <vector>

namespace Arcv {

template <typename T>
class Vec {
public:
  Vec(std::size_t size) : data(size) {}
  Vec(const std::initializer_list<T>& list);

  const std::vector<T>& getData() const { return data; }
  std::vector<T>& getData() { return data; }

  T dot(const Vec& vec) const;

  Vec operator+(Vec vec);
  Vec operator+(float val);
  Vec operator-(Vec vec);
  Vec operator-(float val);
  Vec operator*(Vec vec);
  Vec operator*(float val);
  Vec operator/(Vec vec);
  Vec operator/(float val);
  Vec& operator+=(const Vec& vec);
  Vec& operator+=(float val);
  Vec& operator-=(const Vec& vec);
  Vec& operator-=(float val);
  Vec& operator*=(const Vec& vec);
  Vec& operator*=(float val);
  Vec& operator/=(const Vec& vec);
  Vec& operator/=(float val);
  const T& operator[](const unsigned int index) const { return data[index]; }
  T& operator[](const unsigned int index) { return data[index]; }

private:
  std::vector<T> data;
};

} // namespace Arcv

#include "ArcV/Math/Vec.inl"

#endif // ARCV_VEC_HPP
