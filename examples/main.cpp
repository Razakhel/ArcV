#include <iostream>

#include "ArcV/ArcV.hpp"

int main(int argc, char* argv[]) {
  Arcv::Mat mat = Arcv::Image::read("lena.png");

  Arcv::Image::changeColorspace<ARCV_COLORSPACE_HSV>(mat);
  Arcv::Image::write(mat, "output.png");

  return 0;
}
