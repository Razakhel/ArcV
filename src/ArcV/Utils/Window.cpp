#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "ArcV/Utils/Window.hpp"

namespace Arcv {

Window::Window(const unsigned short width, const unsigned short height) {
  ImGuiIO& io = ImGui::GetIO();
  io.DisplaySize.x = width;
  io.DisplaySize.y = height;

  unsigned char* pixels;
  int w, h;
  io.Fonts->GetTexDataAsRGBA32(&pixels, &w, &h);
}

void Window::show() {
  while (true) {
    ImGui::NewFrame();
    ImGui::Begin("Window");
    ImGui::Text("Test");
    ImGui::End();
    ImGui::Render();
  }
}

Window::~Window() {
}

} // namespace Arcv
