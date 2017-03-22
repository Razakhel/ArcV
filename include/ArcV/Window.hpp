#pragma once

#ifndef ARCV_WINDOW_HPP
#define ARCV_WINDOW_HPP

#include <vector>
#include <cassert>
#include <iostream>
#include <xcb/xcb.h>

namespace Arcv {

class Window {
public:
  Window(const char* display = nullptr, int* screen = nullptr);

  void create(const uint16_t width, const uint16_t height);
  void show();

  ~Window();

private:
  xcb_connection_t* connection;
  xcb_gcontext_t graphicsContext;
  xcb_drawable_t windowId;
};

} // namespace Arcv

#endif // ARCV_WINDOW_HPP
