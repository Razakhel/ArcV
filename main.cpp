#include "Image.hpp"

int main(int argc, char* argv[]) {
  Image image;
  image.writeImage("test.png", Image::Type::PNG);

  return 0;
}
