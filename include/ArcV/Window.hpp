#pragma once

#ifndef ARCV_WINDOW_HPP
#define ARCV_WINDOW_HPP

#include <vector>
#include <xcb/xcb.h>

namespace Arcv {

class Window {
public:
  Window() {}
  Window(const uint16_t width, const uint16_t height);

  void show();

private:
  xcb_connection_t* connection;
};

}

#endif //ARCV_WINDOW_HPP
