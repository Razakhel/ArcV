#pragma once

#ifndef ARCV_WINDOW_HPP
#define ARCV_WINDOW_HPP

#include "ArcV/Processing/Image.hpp"

namespace Arcv {

class Window {
public:
  Window(const unsigned short width, const unsigned short height);

  void show();

  ~Window();
};

} // namespace Arcv

#endif // ARCV_WINDOW_HPP
