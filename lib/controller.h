#pragma once

#include <thread>  // std::this_thread::sleep_for
#include "converter.h"
#include "publisher.h"

namespace sudare {
class controller {
  std::chrono::time_point<std::chrono::system_clock> m_last_sleep_time;
  enum mode_t { rectangular_mode = 0, polar_mode = 1 } m_mode;
  sudare::rectangular m_rect;
  sudare::polar m_polar;
  sudare::bilinear_converter m_conv;
  sudare::publisher& m_pub;

 public:
  explicit controller(sudare::publisher& pub);
  controller(sudare::publisher& pub, size_t rect_width, size_t rect_height,
             size_t rect_depth);
  void set_led_rect(int x, int y, int z, int rgb);
  void set_led_polar(int a, int r, int h, int rgb);
  void clear();
  void send();
  void sleep(int ms);
};
}  // namespace sudare
