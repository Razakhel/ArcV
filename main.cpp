#include "Image.hpp"

int main(int argc, char* argv[]) {
  Image<PNG> img;
  img.read("test3.png");

  return 0;
}
