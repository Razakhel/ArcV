#include "Vec.hpp"
#include "Mat.hpp"
#include "Image.hpp"
#include "Window.hpp"

int main(int argc, char* argv[]) {
  Arcv::Image<Arcv::PNG> img;
  img.read("test3.png");

  Arcv::Window window;
  window.create(150, 150);
  window.show();

  return 0;
}