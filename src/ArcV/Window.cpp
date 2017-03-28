#include "Window.hpp"

namespace Arcv {

Window::Window(const char* display, int* screen) {
  connection = xcb_connect(display, screen);
  assert(("Error: Could not create screen connection (wrong display and/or screen ID).", !xcb_connection_has_error(connection)));
}

void Window::createWindowFrame(const uint16_t width, const uint16_t height) {
  unsigned int mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  std::array<unsigned int, 2> values;
  window = xcb_generate_id(connection);
  values[0] = screen->white_pixel;
  values[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_KEY_PRESS;

  xcb_create_window(connection,
                    XCB_COPY_FROM_PARENT,
                    window,
                    screen->root,                  // Parent window
                    0, 0,                          // Position x & y
                    width, height,
                    0,                            // Border width
                    XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    screen->root_visual,
                    mask,
                    values.data());
}

void Window::createGraphicsContext() {
  unsigned int mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_GRAPHICS_EXPOSURES;
  std::array<unsigned int, 3> values;
  graphicsContext = xcb_generate_id(connection);
  values[0] = screen->black_pixel;
  values[1] = screen->white_pixel;
  values[2] = 0;

  xcb_create_gc(connection,
                graphicsContext,
                window,
                mask,
                values.data());
}

void Window::create(const uint16_t width, const uint16_t height) {
  screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;

  createWindowFrame(width, height);
  createGraphicsContext();

  xcb_map_window(connection, window);
  xcb_flush(connection);
}

void Window::mapImage(const Arcv::Image<PNG>& img) {
  pixmap = xcb_create_pixmap_from_bitmap_data(connection,
                                              window,
                                              img.getData(),
                                              img.getWidth(),
                                              img.getHeight(),
                                              img.getBitDepth(),    // Replace by 24 to see an ugly square full of artifacts
                                              screen->black_pixel,
                                              screen->white_pixel,
                                              nullptr);

  xcb_flush(connection);
}

void Window::show() {
  bool terminate = false;
  xcb_generic_event_t* event;

  // Events handling loop
  while (!terminate && (event = xcb_wait_for_event(connection))) {
    switch (event->response_type) {
      case XCB_EXPOSE: {
        xcb_expose_event_t* exposeEvent = reinterpret_cast<xcb_expose_event_t*>(event);
        xcb_copy_area(connection,
                      pixmap,
                      window,
                      graphicsContext,
                      exposeEvent->x, exposeEvent->y,             // Top left x & y coordinates of the source's region to copy
                      exposeEvent->x, exposeEvent->y,             // Top left x & y coordinates of the destination's region to copy to
                      exposeEvent->width,
                      exposeEvent->height);
        xcb_flush(connection);
        break;
      }

      case XCB_BUTTON_PRESS:
        terminate = true;
        break;

      case XCB_KEY_PRESS:
        break;

      default:
        break;
    }
    delete event;
  }
}

Window::~Window() {
  xcb_free_pixmap(connection, pixmap);
  xcb_destroy_window(connection, window);
  xcb_disconnect(connection);
}

} // namespace Arcv
