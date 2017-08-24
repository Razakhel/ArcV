#include <chrono>
#include <iostream>

#include "ArcV/ArcV.hpp"

int main() {
  auto startTime = std::chrono::system_clock::now();

  Arcv::Matrix<float> mat = Arcv::Image::read("lena.png");

  Arcv::Matrix<float> grayMat = Arcv::Image::changeColorspace<ARCV_COLORSPACE_GRAY>(mat);
  Arcv::Matrix<float> hsvMat = Arcv::Image::changeColorspace<ARCV_COLORSPACE_HSV>(mat);
  Arcv::Matrix<float> rgbaMat = Arcv::Image::changeColorspace<ARCV_COLORSPACE_RGBA>(mat);

  Arcv::Matrix<float> blurMat = Arcv::Image::applyFilter<ARCV_FILTER_TYPE_GAUSSIAN_BLUR>(mat);
  Arcv::Matrix<float> sharpenMat = Arcv::Image::applyFilter<ARCV_FILTER_TYPE_SHARPEN>(mat);
  Arcv::Matrix<float> edgeMat = Arcv::Image::applyFilter<ARCV_FILTER_TYPE_EDGE_ENHANCEMENT>(mat);
  Arcv::Matrix<float> embossMat = Arcv::Image::applyFilter<ARCV_FILTER_TYPE_EMBOSS>(mat);
  Arcv::Matrix<float> sobelMat = Arcv::Image::applyFilter<ARCV_FILTER_TYPE_SOBEL>(mat);

  Arcv::Matrix<float> harrisMat = Arcv::Image::applyDetector<ARCV_DETECTOR_TYPE_HARRIS>(mat);

  Arcv::Image::write(mat, "output.png");

  Arcv::Image::write(grayMat, "outputGray.png");
  Arcv::Image::write(hsvMat, "outputHSV.png");
  Arcv::Image::write(rgbaMat, "outputRGBA.png");

  Arcv::Image::write(blurMat, "outputBlur.png");
  Arcv::Image::write(sharpenMat, "outputSharpen.png");
  Arcv::Image::write(edgeMat, "outputEdge.png");
  Arcv::Image::write(embossMat, "outputEmboss.png");
  Arcv::Image::write(sobelMat, "outputSobel.png");

  Arcv::Image::write(harrisMat, "outputHarris.png");

  Arcv::Matrix<float> threshMat = Arcv::Image::threshold(hsvMat, 0.f, 15.f, 50.f, 150.f, 50.f, 150.f);
  Arcv::Image::write(threshMat, "outputThresh.png");

  Arcv::Matrix<float> leftRotMat = Arcv::Image::rotateLeft(mat);
  Arcv::Matrix<float> rightRotMat = Arcv::Image::rotateRight(mat);
  Arcv::Matrix<float> reverseMat = Arcv::Image::reverse(mat);
  Arcv::Image::write(leftRotMat, "outputLeftRot.png");
  Arcv::Image::write(rightRotMat, "outputRightRot.png");
  Arcv::Image::write(reverseMat, "outputReverse.png");

  Arcv::Matrix<float> horizFlipMat = Arcv::Image::horizontalFlip(mat);
  Arcv::Matrix<float> vertFlipMat = Arcv::Image::verticalFlip(mat);
  Arcv::Image::write(horizFlipMat, "outputHorizFlip.png");
  Arcv::Image::write(vertFlipMat, "outputVertFlip.png");

  Arcv::Matrix<float> regionMat = Arcv::Image::region(mat, 0, mat.getWidth() / 2, 0, mat.getHeight() / 2);
  Arcv::Image::write(regionMat, "outputRegion.png");

  std::cout << "Computing duration: "
    << std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::system_clock::now() - startTime).count()
    << " seconds." << std::endl;

  return EXIT_SUCCESS;
}
