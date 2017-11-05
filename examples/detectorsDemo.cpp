#include <chrono>
#include <iostream>

#include "ArcV/ArcV.hpp"

int main() {
  const auto startTime = std::chrono::system_clock::now();

  Arcv::Matrix<float> mat = Arcv::Image::read("../../assets/lena.png");

  Arcv::Matrix<float> cannyMat = Arcv::Image::applyDetector<ARCV_DETECTOR_TYPE_CANNY>(mat);

  Arcv::Image::write(mat, "output.png");
  Arcv::Image::write(cannyMat, "outputCanny.png");

  const auto endTime = std::chrono::system_clock::now();

  std::cout << "Computing duration: "
            << std::chrono::duration_cast<std::chrono::duration<float>>(endTime - startTime).count()
            << " seconds." << std::endl;

  return EXIT_SUCCESS;
}
