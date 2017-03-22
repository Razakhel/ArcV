#include "Window.hpp"

namespace Arcv {

Window::Window(const char* display, int* screen) {
  connection = xcb_connect(display, screen);
  assert(("Error: Could not create screen connection (wrong display and/or screen ID).", !xcb_connection_has_error(connection)));
}

void Window::create(const uint16_t width, const uint16_t height) {
  xcb_screen_t* screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;
  unsigned int mask = 0;
  std::vector<unsigned int> values(2);

  xcb_drawable_t window = xcb_generate_id(connection);
  mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  values[0] = screen->white_pixel;
  values[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS;
  xcb_create_window(connection,
                    XCB_COPY_FROM_PARENT,
                    window,                        // Window's ID
                    screen->root,                  // Parent window
                    0, 0,                          // Position x & y
                    width, height,
                    10,                            // Border width
                    XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    screen->root_visual,
                    mask,
                    values.data());

  xcb_map_window(connection, window);
  xcb_flush(connection);
}

void Window::show() {
  // Events handling loop
  xcb_generic_event_t* event;
  while ((event = xcb_wait_for_event(connection))) {
    switch (event->response_type) {
      case XCB_EXPOSE:
        xcb_flush(connection);
        break;

      case XCB_KEY_PRESS:
        return;

      default:
        break;
    }
    delete event;
  }
}

} // namespace Arcv