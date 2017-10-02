#pragma once

#ifndef ARCV_VEC_HPP
#define ARCV_VEC_HPP

#include <vector>

namespace Arcv {

template <typename T = float>
class Vector {
public:
  Vector(std::size_t size) : data(size) {}
  Vector(std::initializer_list<T> list);

  const std::vector<T>& getData() const { return data; }
  std::vector<T>& getData() { return data; }

  T dot(const Vector& vec) const;
  Vector normalize() const;

  Vector operator+(Vector vec);
  Vector operator+(float val);
  Vector operator-(Vector vec);
  Vector operator-(float val);
  Vector operator*(Vector vec);
  Vector operator*(float val);
  Vector operator/(Vector vec);
  Vector operator/(float val);
  Vector& operator+=(const Vector& vec);
  Vector& operator+=(float val);
  Vector& operator-=(const Vector& vec);
  Vector& operator-=(float val);
  Vector& operator*=(const Vector& vec);
  Vector& operator*=(float val);
  Vector& operator/=(const Vector& vec);
  Vector& operator/=(float val);
  const T& operator[](std::size_t index) const { return data[index]; }
  T& operator[](std::size_t index) { return data[index]; }

private:
  std::vector<T> data;
};

using Vec = Vector<>;

} // namespace Arcv

#include "ArcV/Math/Vector.inl"

#endif // ARCV_VEC_HPP
