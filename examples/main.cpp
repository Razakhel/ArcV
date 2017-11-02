#include <chrono>
#include <iostream>

#include "ArcV/ArcV.hpp"

int main() {
  const auto startTime = std::chrono::system_clock::now();

  const Arcv::Matrix<float> mat = Arcv::Image::read("../assets/lena.png");

  const Arcv::Sobel sobelMat = Arcv::Sobel(mat);
  const Arcv::Matrix<float> cannyMat = Arcv::Image::applyDetector<ARCV_DETECTOR_TYPE_CANNY>(mat);

  Arcv::Image::write(sobelMat.getSobelMat(), "outputSobel.png");
  Arcv::Image::write(sobelMat.getHorizontalGradient(), "outputSobelHoriz.png");
  Arcv::Image::write(sobelMat.getVerticalGradient(), "outputSobelVert.png");
  Arcv::Image::write(sobelMat.computeGradientDirection(), "outputSobelDirection.png");
  Arcv::Image::write(cannyMat, "outputCanny.png");

  Arcv::Webcam cam(cannyMat.getWidth(), cannyMat.getHeight());

  const auto endTime = std::chrono::system_clock::now();

  std::cout << "Computing duration: "
    << std::chrono::duration_cast<std::chrono::duration<float>>(endTime - startTime).count()
    << " seconds." << std::endl;

  Arcv::Window window(mat.getWidth(), mat.getHeight(), "Example");
  window.mapImage(cannyMat);
  window.show();

  return EXIT_SUCCESS;
}
