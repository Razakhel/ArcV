#pragma once

#ifndef ARCV_WINDOW_HPP
#define ARCV_WINDOW_HPP

#include <xcb/xcb.h>

#include "Image.hpp"

namespace Arcv {

class Window {
public:
  Window(const char* display = nullptr, int* screen = nullptr);

  void create(const uint16_t width, const uint16_t height);
  void mapImage(Arcv::Image<PNG>& img);
  void show();

  ~Window();

private:
  void createWindowFrame(const uint16_t width, const uint16_t height);
  void createGraphicsContext();

  xcb_connection_t* connection;
  xcb_screen_t* screen;
  xcb_gcontext_t graphicsContext;
  xcb_window_t window;
  xcb_pixmap_t pixmap;
};

} // namespace Arcv

#endif // ARCV_WINDOW_HPP
