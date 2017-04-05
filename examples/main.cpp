#include <iostream>

#include "ArcV.hpp"

int main(int argc, char* argv[]) {
  Arcv::Mat mat = Arcv::Image<Arcv::PNG>::read("test3.png");
  Arcv::Image<Arcv::PNG>::write(mat, "testOut.png");

  return 0;
}
