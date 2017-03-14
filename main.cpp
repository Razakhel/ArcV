#include "Image.hpp"

int main(int argc, char* argv[]) {
  Image img;
  img.read("test.png");
  //image.write("test2.png", Image::Type::PNG);

  return 0;
}
