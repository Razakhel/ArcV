#include <iostream>

#include "ArcV/ArcV.hpp"

int main(int argc, char* argv[]) {
  Arcv::Mat mat = Arcv::Image::read("lena.png");   // Arcv::Mat is an Arcv::Matrix<> (<uint8_t> by default)

  Arcv::Image::applyFilter<ARCV_FILTER_TYPE_GAUSSIAN_BLUR>(mat);

  Arcv::Image::write(mat, "output.png");

  return EXIT_SUCCESS;
}
