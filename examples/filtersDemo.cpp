#include <chrono>
#include <iostream>

#include "ArcV/ArcV.hpp"

int main() {
  const auto startTime = std::chrono::system_clock::now();

  Arcv::Matrix<float> mat = Arcv::Image::read("../../assets/lena.png");

  Arcv::Matrix<float> blurMat = Arcv::Image::applyFilter<ARCV_FILTER_TYPE_GAUSSIAN_BLUR>(mat);
  Arcv::Matrix<float> sharpenMat = Arcv::Image::applyFilter<ARCV_FILTER_TYPE_SHARPEN>(mat);
  Arcv::Matrix<float> edgeMat = Arcv::Image::applyFilter<ARCV_FILTER_TYPE_EDGE_ENHANCEMENT>(mat);
  Arcv::Matrix<float> embossMat = Arcv::Image::applyFilter<ARCV_FILTER_TYPE_EMBOSS>(mat);

  Arcv::Sobel sobelMat(mat);

  Arcv::Image::write(mat, "output.png");
  Arcv::Image::write(blurMat, "outputBlur.png");
  Arcv::Image::write(sharpenMat, "outputSharpen.png");
  Arcv::Image::write(edgeMat, "outputEdge.png");
  Arcv::Image::write(embossMat, "outputEmboss.png");

  Arcv::Image::write(sobelMat.getSobelMat(), "outputSobel.png");
  Arcv::Image::write(sobelMat.getHorizontalGradient(), "outputSobelHoriz.png");
  Arcv::Image::write(sobelMat.getVerticalGradient(), "outputSobelVert.png");
  Arcv::Image::write(sobelMat.computeGradientDirection(), "outputSobelDirection.png");

  const auto endTime = std::chrono::system_clock::now();

  std::cout << "Computing duration: "
            << std::chrono::duration_cast<std::chrono::duration<float>>(endTime - startTime).count()
            << " seconds." << std::endl;

  return EXIT_SUCCESS;
}
