#include <chrono>
#include <iostream>

#include "ArcV/ArcV.hpp"

int main() {
#ifdef __gnu_linux__
  const Arcv::Webcam cam(640, 480);

  const Arcv::Window window(cam.getFrameWidth(), cam.getFrameHeight(), "Example");

  while (window.show())
    window.mapImage(cam.captureImage());
#else
  const auto startTime = std::chrono::system_clock::now();

  const Arcv::Matrix<float> mat = Arcv::Image::applyDetector<ARCV_DETECTOR_TYPE_CANNY>(Arcv::Image::read("../assets/lena.png"));

  const auto endTime = std::chrono::system_clock::now();

  std::cout << "Computing duration: "
            << std::chrono::duration_cast<std::chrono::duration<float>>(endTime - startTime).count()
            << " seconds." << std::endl;

  Arcv::Image::write(mat, "outputCanny.png");
#endif

  return EXIT_SUCCESS;
}
