#include "Vec.hpp"
#include "Mat.hpp"
#include "Image.hpp"

int main(int argc, char* argv[]) {
  Vec3f vec1(3.f, 5.f, 8.f);
  Vec3f vec2(9.f, 15.f, 88.f);

  Image<PNG> img;
  img.read("test3.png");

  return 0;
}
