#include "sudare.h"
#include <zmq.h>
#include <iostream>
#include <memory>  // std::shared_ptr
#include <thread>  // std::this_thread::sleep_for
#include "converter.hpp"
#include "publisher.hpp"

namespace {
class TimeMeter {
  std::chrono::time_point<std::chrono::system_clock> m_start;
  std::string m_symbol;

 public:
  explicit TimeMeter(const char* symbol)
      : m_start(std::chrono::system_clock::now()), m_symbol(symbol) {}
  virtual ~TimeMeter() {
    auto end = std::chrono::system_clock::now();
    int diff = static_cast<int>(
        std::chrono::duration_cast<std::chrono::milliseconds>(end - m_start)
            .count());
    std::cout << m_symbol << " : " << diff << "ms" << std::endl;
  }
};
class Sudare {
  std::chrono::time_point<std::chrono::system_clock> m_last_sleep_time;
  enum mode_t { rectangular_mode = 0, polar_mode = 1 } m_mode;
  sudare::rectangular m_rect;
  sudare::polar m_polar;
  sudare::bilinear m_conv;
  sudare::publisher& m_pub;

 public:
  explicit Sudare(sudare::publisher& pub)
      : m_last_sleep_time(std::chrono::system_clock::now()),
        m_mode(rectangular_mode),
        m_rect(RECTANGULAR_WIDTH, RECTANGULAR_HEIGHT, RECTANGULAR_DEPTH),
        m_polar(POLAR_ANGLES, POLAR_RADIUS, POLAR_HEIGHT),
        m_conv(m_rect, m_polar),
        m_pub(pub) {}
  Sudare(sudare::publisher& pub, size_t rect_width, size_t rect_height,
         size_t rect_depth)
      : m_last_sleep_time(std::chrono::system_clock::now()),
        m_mode(rectangular_mode),
        m_rect(rect_width, rect_height, rect_depth),
        m_polar(POLAR_ANGLES, POLAR_RADIUS, POLAR_HEIGHT),
        m_conv(m_rect, m_polar),
        m_pub(pub) {}
  void set_led_rect(int x, int y, int z, int rgb) {
    m_rect.set(x, y, z, rgb);
    m_mode = rectangular_mode;
  }
  void set_led_polar(int a, int r, int h, int rgb) {
    m_polar.set(a, r, h, rgb);
    m_mode = polar_mode;
  }
  void clear() {
    m_rect.clear();
    m_polar.clear();
  }
  void send() {
    if (m_mode == rectangular_mode) {
      TimeMeter t("convert");
      m_conv();
    }
    TimeMeter t("publish");
    m_pub.publish(reinterpret_cast<const char*>(m_polar.data()),
                  m_polar.size());
  }
  void sleep(int ms) {
    std::chrono::time_point now = std::chrono::system_clock::now();
    int diff =
        static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(
                             now - m_last_sleep_time)
                             .count());
    ms = std::max(1, ms - diff);
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    m_last_sleep_time = std::chrono::system_clock::now();
  }
};
std::shared_ptr<sudare::zmq_sender> s_sender;
std::shared_ptr<Sudare> s;
}  // namespace

int sudare_init_sdk(const char* dst) {
  try {
    s_sender = std::make_shared<sudare::zmq_sender>(dst);
    s = std::make_shared<Sudare>(*s_sender);
    return 0;
  } catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

int sudare_set_led_rect(int x, int y, int z, int rgb) {
  try {
    s->set_led_rect(x, y, z, rgb);
    return 0;
  } catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

int sudare_set_led_polar(int a, int r, int h, int rgb) {
  try {
    s->set_led_polar(a, r, h, rgb);
    return 0;
  } catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

int sudare_clear(void) {
  try {
    s->clear();
    return 0;
  } catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

int sudare_send(void) {
  try {
    s->send();
    return 0;
  } catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

int sudare_sleep(int ms) {
  try {
    s->sleep(ms);
    return 0;
  } catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
