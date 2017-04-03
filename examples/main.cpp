#include <iostream>

#include "ArcV.hpp"

int main(int argc, char* argv[]) {
  Arcv::Image<Arcv::PNG> img;
  img.read("test.png");
  img.write("testOut.png");

  Arcv::Mat mat(3000, 3000);

  Arcv::Vec vec(3);
  for (unsigned int i = 0; i < vec.getData().size(); ++i)
    vec[i] = i * 2;
  vec *= 3;

  Arcv::Vec vec2(3);
  for (unsigned int i = 0; i < vec2.getData().size(); ++i)
    vec2[i] = i * 3;

  std::cout << vec.dot(vec2) << std::endl; // 90

  return 0;
}
