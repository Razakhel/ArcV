#include <iostream>

#include "ArcV/ArcV.hpp"

int main(int argc, char* argv[]) {
  Arcv::Mat mat = Arcv::Image::read("lena.png");   // Arcv::Mat is an Arcv::Matrix<> (<uint8_t> by default)

  Arcv::Matrix<float> convol =  {{ -1.f, -1.f, -1.f },
                                 { -1.f,  8.f, -1.f },
                                 { -1.f, -1.f, -1.f }};
  mat.convolve(convol);

  Arcv::Image::write(mat, "output.png");

  return EXIT_SUCCESS;
}
