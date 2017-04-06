#include <iostream>

#include "ArcV/ArcV.hpp"

int main(int argc, char* argv[]) {
  Arcv::Mat mat = Arcv::Image<Arcv::PNG>::read("test3.png");

  mat /= 0.5;
  Arcv::Image<Arcv::PNG>::write(mat, "testOut.png");

  return 0;
}
