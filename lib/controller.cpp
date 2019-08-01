#include "controller.h"
#include <iostream>

namespace sudare {
controller::controller(sudare::converter const& conv,
                       sudare::publisher const& pub)
    : m_last_sleep_time(std::chrono::system_clock::now()),
      m_mode(rectangular_mode),
      m_rect(RECTANGULAR_WIDTH, RECTANGULAR_HEIGHT, RECTANGULAR_DEPTH),
      m_polar(POLAR_ANGLES, POLAR_RADIUS, POLAR_HEIGHT),
      m_conv(conv),
      m_pub(pub) {}
controller::controller(sudare::converter const& conv,
                       sudare::publisher const& pub, int rect_width,
                       int rect_height, int rect_depth)
    : m_last_sleep_time(std::chrono::system_clock::now()),
      m_mode(rectangular_mode),
      m_rect(rect_width, rect_height, rect_depth),
      m_polar(POLAR_ANGLES, POLAR_RADIUS, POLAR_HEIGHT),
      m_conv(conv),
      m_pub(pub) {}
void controller::set_led_rect(int x, int y, int z, int rgb) {
  m_rect.set(x, y, z, rgb);
  m_mode = rectangular_mode;
}
void controller::set_led_polar(int a, int r, int h, int rgb) {
  m_polar.set(a, r, h, rgb);
  m_mode = polar_mode;
}
void controller::clear() {
  m_rect.clear();
  m_polar.clear();
}
void controller::send() {
  if (m_mode == rectangular_mode) m_conv(m_rect, m_polar);
  m_pub(m_polar);
}
void controller::sleep(int ms) {
  auto now = std::chrono::system_clock::now();
  int diff =
      static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(
                           now - m_last_sleep_time)
                           .count());
  ms = std::max(1, ms - diff);
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
  m_last_sleep_time = std::chrono::system_clock::now();
}
}  // namespace sudare