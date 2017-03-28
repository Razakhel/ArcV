#include "Vec.hpp"
#include "Mat.hpp"
#include "Image.hpp"
#include "Window.hpp"

int main(int argc, char* argv[]) {
  Arcv::Image<Arcv::PNG> img;
  img.read("test.png");
  img.write("testOut.png");

  Arcv::Mat mat(3000, 3000);

  Arcv::Window window;
  window.create(1000, 1000);
  window.mapImage(img);
  window.show();

  return 0;
}
