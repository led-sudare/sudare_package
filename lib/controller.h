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
  sudare::converter const& m_conv;
  sudare::publisher const& m_pub;

 public:
  controller(sudare::converter const& conv, sudare::publisher const& pub);
  controller(sudare::converter const& conv, sudare::publisher const& pub,
             int rect_width, int rect_height, int rect_depth);
  void set_led_rect(int x, int y, int z, int rgb);
  void set_led_polar(int a, int r, int h, int rgb);
  void clear();
  void send();
  void sleep(int ms);
};
}  // namespace sudare
