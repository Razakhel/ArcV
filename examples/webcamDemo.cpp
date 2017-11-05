#include <chrono>
#include <iostream>

#include "ArcV/ArcV.hpp"

int main() {
  const Arcv::Webcam cam(640, 480);

  const Arcv::Window window(cam.getFrameWidth(), cam.getFrameHeight(), "Example");

  while (window.show())
    window.mapImage(cam.captureImage());

  return EXIT_SUCCESS;
}
