#include <array>
#include <iostream>

#include "GL/glew.h"
#include "ArcV/Utils/Window.hpp"

namespace Arcv {

namespace {

const std::array<float, 20> vertices = { -1.f, -1.f, 0.f,   0.f, 1.f,
                                          1.f, -1.f, 0.f,   1.f, 1.f,
                                          1.f,  1.f, 0.f,   1.f, 0.f,
                                         -1.f,  1.f, 0.f,   0.f, 0.f };

const std::array<unsigned int, 6> indices = { 0, 1, 3,
                                              1, 2, 3 };

GLuint initShaders() {
  const std::string vertexShaderStr = R"(
    #version 330 core

    layout (location = 0) in vec3 vertPosition;
    layout (location = 1) in vec2 vertTexcoords;

    out vec2 fragTexcoords;

    void main() {
        gl_Position = vec4(vertPosition, 1.0);
        fragTexcoords = vertTexcoords;
    }
  )";

  const std::string fragmentShaderStr = R"(
    #version 330 core

    uniform sampler2D uniTexture;

    in vec2 fragTexcoords;

    void main() {
        gl_FragColor = texture(uniTexture, fragTexcoords);
    }
  )";

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, reinterpret_cast<const GLchar* const*>(&vertexShaderStr), nullptr);
  glCompileShader(vertexShader);

  GLint success;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    std::array<GLchar, 512> infoLog;

    glGetShaderInfoLog(vertexShader, infoLog.size(), nullptr, infoLog.data());
    std::cerr << "Error: Vertex shader compilation failed.\n" << infoLog.data() << std::endl;
  }

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, reinterpret_cast<const GLchar* const*>(&fragmentShaderStr), nullptr);
  glCompileShader(fragmentShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    std::array<GLchar, 512> infoLog;

    glGetShaderInfoLog(vertexShader, infoLog.size(), nullptr, infoLog.data());
    std::cerr << "Error: Fragment shader compilation failed.\n" << infoLog.data() << std::endl;
  }

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

  if (!success) {
    std::array<GLchar, 512> infoLog;

    glGetProgramInfoLog(shaderProgram, infoLog.size(), nullptr, infoLog.data());
    std::cerr << "Error: Shader program link failed.\n" << infoLog.data() << std::endl;
  }

  return shaderProgram;
}

} // namespace

Window::Window(unsigned int width, unsigned int height, const std::string& name) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
  if (!window) {
    std::cerr << "Error: Failed to create GLFW Window." << std::endl;
    glfwTerminate();
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, [] (GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
  });

  glViewport(0, 0, width, height);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
    std::cerr << "Error: Failed to initialize GLEW." << std::endl;

  glfwMakeContextCurrent(window);
  glViewport(0, 0, width, height);

  shaderProgram = initShaders();

  glGenVertexArrays(1, &vaoIndex);
  glGenBuffers(1, &vboIndex);
  glGenBuffers(1, &eboIndex);
  glGenTextures(1, &textureIndex);

  glBindVertexArray(vaoIndex);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboIndex);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ARRAY_BUFFER, vboIndex);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glBindTexture(GL_TEXTURE_2D, textureIndex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Window::mapImage(const Matrix<uint8_t>& mat) const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboIndex);

  GLenum imgFormat;
  switch (mat.getColorspace()) {
    case ARCV_COLORSPACE_GRAY: {
      const std::array<GLint, 4> swizzle = { GL_RED, GL_RED, GL_RED, GL_ONE };
      glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle.data());

      imgFormat = GL_RED;
      break;
    }

    case ARCV_COLORSPACE_GRAY_ALPHA: {
      const std::array<GLint, 4> swizzle = { GL_RED, GL_RED, GL_RED, GL_GREEN };
      glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle.data());

      imgFormat = GL_RG;
      break;
    }

    case ARCV_COLORSPACE_RGBA:
      imgFormat = GL_RGBA;
      break;

    default:
      imgFormat = GL_RGB;
      break;
  }

  glTexImage2D(GL_TEXTURE_2D, 0, imgFormat, mat.getWidth(), mat.getHeight(), 0, imgFormat, GL_UNSIGNED_BYTE, mat.getData().data());
}

bool Window::show() const {
  if (glfwWindowShouldClose(window))
    return false;

  glClear(GL_COLOR_BUFFER_BIT);

  glBindTexture(GL_TEXTURE_2D, textureIndex);

  glUseProgram(shaderProgram);
  glBindVertexArray(vaoIndex);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

  glfwSwapBuffers(window);
  glfwPollEvents();

  return true;
}

void Window::close() const {
  glDeleteVertexArrays(1, &vaoIndex);
  glDeleteBuffers(1, &vboIndex);
  glDeleteBuffers(1, &eboIndex);

  glfwTerminate();
}

} // namespace Arcv
