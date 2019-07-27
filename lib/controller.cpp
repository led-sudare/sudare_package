#include "controller.h"
#include <iostream>

namespace sudare {
namespace {
class time_meter {
  std::chrono::time_point<std::chrono::system_clock> m_start;
  std::string m_symbol;

 public:
  explicit time_meter(const char* symbol)
      : m_start(std::chrono::system_clock::now()), m_symbol(symbol) {}
  virtual ~time_meter() {
    auto end = std::chrono::system_clock::now();
    int diff = static_cast<int>(
        std::chrono::duration_cast<std::chrono::milliseconds>(end - m_start)
            .count());
    std::cout << m_symbol << " : " << diff << "ms" << std::endl;
  }
};
}  // namespace

controller::controller(sudare::publisher& pub)
    : m_last_sleep_time(std::chrono::system_clock::now()),
      m_mode(rectangular_mode),
      m_rect(RECTANGULAR_WIDTH, RECTANGULAR_HEIGHT, RECTANGULAR_DEPTH),
      m_polar(POLAR_ANGLES, POLAR_RADIUS, POLAR_HEIGHT),
      m_conv(m_rect, m_polar),
      m_pub(pub) {}
controller::controller(sudare::publisher& pub, size_t rect_width,
                       size_t rect_height, size_t rect_depth)
    : m_last_sleep_time(std::chrono::system_clock::now()),
      m_mode(rectangular_mode),
      m_rect(rect_width, rect_height, rect_depth),
      m_polar(POLAR_ANGLES, POLAR_RADIUS, POLAR_HEIGHT),
      m_conv(m_rect, m_polar),
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
  if (m_mode == rectangular_mode) {
    time_meter t("convert");
    m_conv();
  }
  time_meter t("publish");
  m_pub(m_polar.data(), m_polar.size());
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