#pragma once

#ifndef ARCV_WINDOW_HPP
#define ARCV_WINDOW_HPP

#include "GLFW/glfw3.h"
#include "ArcV/Processing/Image.hpp"

namespace Arcv {

class Window {
public:
  Window(unsigned int width, unsigned int height, const std::string& name = "");

  void mapImage(Matrix<uint8_t> mat);
  void show() const;
  void close() const;

  ~Window() { close(); }

private:
  GLFWwindow* window;

  GLuint vaoIndex;
  GLuint vboIndex;
  GLuint eboIndex;
  GLuint shaderProgram;
  GLuint textureIndex;
};

} // namespace Arcv

#endif // ARCV_WINDOW_HPP
