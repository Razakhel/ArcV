#include <iostream>

#include "ArcV/ArcV.hpp"

int main(int argc, char* argv[]) {
  Arcv::Matrix<float> mat = Arcv::Image::read("lena.png");

  //Arcv::Image::changeColorspace<ARCV_COLORSPACE_GRAY>(mat);
  //Arcv::Image::applyFilter<ARCV_FILTER_TYPE_GAUSSIAN_BLUR>(mat);

  Arcv::Image::write(mat, "output.png");

  return EXIT_SUCCESS;
}
