#include <chrono>
#include <iostream>

#include "ArcV/ArcV.hpp"

int main() {
  const auto startTime = std::chrono::system_clock::now();

  Arcv::Matrix<float> mat = Arcv::Image::read("../../assets/lena.png");

  Arcv::Matrix<float> grayMat = Arcv::Image::changeColorspace<ARCV_COLORSPACE_GRAY>(mat);
  Arcv::Matrix<float> hsvMat = Arcv::Image::changeColorspace<ARCV_COLORSPACE_HSV>(mat);
  Arcv::Matrix<float> rgbaMat = Arcv::Image::changeColorspace<ARCV_COLORSPACE_RGBA>(mat);

  Arcv::Image::write(mat, "output.png");
  Arcv::Image::write(grayMat, "outputGray.png");
  Arcv::Image::write(hsvMat, "outputHSV.png");
  Arcv::Image::write(rgbaMat, "outputRGBA.png");

  const auto endTime = std::chrono::system_clock::now();

  std::cout << "Computing duration: "
            << std::chrono::duration_cast<std::chrono::duration<float>>(endTime - startTime).count()
            << " seconds." << std::endl;

  return EXIT_SUCCESS;
}
