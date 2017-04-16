#include <iostream>

#include "ArcV/ArcV.hpp"

int main(int argc, char* argv[]) {
  Arcv::Mat mat = Arcv::Image::read("lena.png");   // Arcv::Mat is an Arcv::Matrix<> (<uint8_t> by default)

  Arcv::Matrix<float> convol =  {{ 1.f,   2.f,  3.f },
                                 { 4.f,   5.f,  6.f },
                                 { 7.f,   8.f,  9.f },
                                 { 10.f, 11.f, 12.f },
                                 { 13.f, 14.f, 15.f },
                                 { 16.f, 17.f, 18.f },
                                 { 19.f, 20.f, 21.f }};

  mat.convolve(convol);

  Arcv::Image::changeColorspace<ARCV_COLORSPACE_HSV>(mat);
  Arcv::Image::write(mat, "output.png");

  return 0;
}
