#include "ArcV/Processing/Image.hpp"

namespace Arcv {

namespace Image {

template <>
Matrix<> applyFilter<ARCV_FILTER_TYPE_GAUSSIAN_BLUR>(Matrix<> mat) {
  Matrix<float> kernel = {{ 1.f,  4.f,  6.f,  4.f, 1.f },
                          { 4.f, 16.f, 24.f, 16.f, 4.f },
                          { 6.f, 24.f, 36.f, 24.f, 6.f },
                          { 4.f, 16.f, 24.f, 16.f, 4.f },
                          { 1.f,  4.f,  6.f,  4.f, 1.f }};

  return mat.convolve(kernel / 256);
}

template <>
Matrix<> applyFilter<ARCV_FILTER_TYPE_SHARPEN>(Matrix<> mat) {
  const Matrix<float> kernel = {{  0.f,  -1.f,  0.f },
                                { -1.f,   5.f, -1.f },
                                {  0.f,  -1.f,  0.f }};

  return mat.convolve(kernel);
}

template <>
Matrix<> applyFilter<ARCV_FILTER_TYPE_EDGE_ENHANCEMENT>(Matrix<> mat) {
  const Matrix<float> kernel = {{ 0.f,   1.f,  0.f },
                                { 1.f,  -4.f,  1.f },
                                { 0.f,   1.f,  0.f }};

  return mat.convolve(kernel);
}

template <>
Matrix<> applyFilter<ARCV_FILTER_TYPE_EMBOSS>(Matrix<> mat) {
  const Matrix<float> kernel = {{ -2.f, -1.f, 0.f },
                                { -1.f,  1.f, 1.f },
                                {  0.f,  1.f, 2.f }};

  return mat.convolve(kernel);
}

} // namespace Image

} // namespace Arcv
