#include "Window.hpp"

namespace Arcv {

Window::Window(const char* display, int* screen) {
  connection = xcb_connect(display, screen);
  assert(("Error: Could not create screen connection (wrong display and/or screen ID).", !xcb_connection_has_error(connection)));
}

void Window::create(const uint16_t width, const uint16_t height) {
  // Creating basic window
  xcb_screen_t* screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;
  unsigned int mask;
  std::vector<unsigned int> values(2);

  // Creating graphics context
  graphicsContext = xcb_generate_id(connection);
  mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
  values[0] = screen->black_pixel;
  values[1] = 0;
  xcb_create_gc(connection,
                graphicsContext,
                screen->root,
                mask,
                values.data());

  windowId = xcb_generate_id(connection);
  mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  values[0] = screen->white_pixel;
  values[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_KEY_PRESS;
  xcb_create_window(connection,
                    XCB_COPY_FROM_PARENT,
                    windowId,
                    screen->root,                  // Parent window
                    0, 0,                          // Position x & y
                    width, height,
                    0,                            // Border width
                    XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    screen->root_visual,
                    mask,
                    values.data());

  xcb_map_window(connection, windowId);
  xcb_flush(connection);
}

void Window::show() {
  bool terminate = false;
  xcb_generic_event_t* event;

  // Events handling loop
  while (!terminate && (event = xcb_wait_for_event(connection))) {
    switch (event->response_type) {
      case XCB_EXPOSE:
        xcb_flush(connection);
        break;

      case XCB_BUTTON_PRESS:
        terminate = true;
        break;

      /*case XCB_KEY_PRESS:
        break;*/

      default:
        break;
    }
    delete event;
  }
}

Window::~Window() {
  xcb_disconnect(connection);
}

} // namespace Arcv