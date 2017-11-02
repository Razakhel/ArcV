#include <chrono>
#include <iostream>

#include "ArcV/ArcV.hpp"

int main() {
  const auto startTime = std::chrono::system_clock::now();

#ifdef __gnu_linux
  const Arcv::Webcam cam(640, 480);
  const Arcv::Matrix<float> mat = cam.captureImage();

  Arcv::Image::write(mat, "outputCam.png");
#else
  const Arcv::Matrix<float> mat = Arcv::Image::applyDetector<ARCV_DETECTOR_TYPE_CANNY>(Arcv::Image::read("../assets/lena.png"));

  Arcv::Image::write(mat, "outputCanny.png");
#endif

  const auto endTime = std::chrono::system_clock::now();

  std::cout << "Computing duration: "
            << std::chrono::duration_cast<std::chrono::duration<float>>(endTime - startTime).count()
            << " seconds." << std::endl;

  Arcv::Window window(mat.getWidth(), mat.getHeight(), "Example");
  window.mapImage(mat);
  window.show();

  return EXIT_SUCCESS;
}
