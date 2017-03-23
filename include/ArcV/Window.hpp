#pragma once

#ifndef ARCV_WINDOW_HPP
#define ARCV_WINDOW_HPP

#include <vector>
#include <cassert>
#include <iostream>
#include <xcb/xcb.h>
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>

#include "Image.hpp"

namespace Arcv {

class Window {
public:
  Window(const char* display = nullptr, int* screen = nullptr);

  void create(const uint16_t width, const uint16_t height);
  void mapImage(const Arcv::Image<PNG>& img);
  void show();

  ~Window();

private:
  xcb_connection_t* connection;
  xcb_colormap_t colormap;
  xcb_gcontext_t graphicsContext;
  xcb_drawable_t windowId;
  xcb_pixmap_t pixmapId;
};

} // namespace Arcv

#endif // ARCV_WINDOW_HPP
