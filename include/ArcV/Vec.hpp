#pragma once

#ifndef ARCV_VEC_HPP
#define ARCV_VEC_HPP

template <typename T, unsigned int Size>
class Vec {
public:
  Vec() : x{ T{0} }, y{ T{0} }, z{ T{0} } {}
  Vec(const T& x, const T& y, const T& z) : x{x}, y{y}, z{z} {}
  Vec(const Vec& vec) {}

  T dot(const Vec& vec) { return x * vec.x + y * vec.y + z * vec.z; }

  Vec<T, Size>& operator+(const Vec& vec) const { return Vec<T, Size>(x + vec.x, y + vec.y, z + vec.z); }
  Vec<T, Size>& operator-(const Vec& vec) const { return Vec<T, Size>(x - vec.x, y - vec.y, z - vec.z); }
  Vec<T, Size>& operator*(const Vec& vec) const { return Vec<T, Size>(x * vec.x, y * vec.y, z * vec.z); }
  Vec<T, Size>& operator*(const T& coeff) const { return Vec<T, Size>(x * coeff, y * coeff, z * coeff); }

private:
  T x, y, z;
};

template <typename T>
using Vec3 = Vec<T, 3>;
using Vec3i = Vec3<int>;
using Vec3u = Vec3<unsigned int>;
using Vec3f = Vec3<float>;

#endif // ARCV_VEC_HPP
