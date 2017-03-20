#include "Image.hpp"

int main(int argc, char* argv[]) {
  Image<PNG> img;
  img.read("test.png");

  return 0;
}
