#include <iostream>

#include "ArcV/ArcV.hpp"

int main(int argc, char* argv[]) {
  Arcv::Matrix<float> mat = Arcv::Image::read("lena.png");

  Arcv::Image::applyDetector<ARCV_DETECTOR_TYPE_HARRIS>(mat);

  Arcv::Image::write(mat, "output.png");

  return EXIT_SUCCESS;
}
