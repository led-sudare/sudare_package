#include "sudare.h"
#include <zmq.h>
#include <iostream>
#include <memory>  // std::shared_ptr
#include <thread>  // std::this_thread::sleep_for
#include "converter.hpp"
#include "zmq_utils.h"

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
  enum mode { rectangular_mode = 0, polar_mode = 1 } m_mode;
  Rectangular m_rect;
  Polar m_polar;
  Converter m_conv;
  zmq_initializer m_zmq_init;
  zmq_client m_zmq_client;

 public:
  explicit Sudare(const char* dst)
      : m_last_sleep_time(std::chrono::system_clock::now()),
        m_mode(rectangular_mode),
        m_rect(RECTANGULAR_WIDTH, RECTANGULAR_HEIGHT, RECTANGULAR_DEPTH),
        m_polar(POLAR_ANGLES, POLAR_RADIUS, POLAR_HEIGHT),
        m_conv(m_rect, m_polar),
        m_zmq_init(),
        m_zmq_client(m_zmq_init.context(), dst) {}
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
    TimeMeter t("zmq send");
    m_zmq_client.send(reinterpret_cast<const char*>(m_polar.data()),
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
std::shared_ptr<Sudare> sudare;
}  // namespace

int sudare_init_sdk(const char* dst) {
  try {
    sudare = std::make_shared<Sudare>(dst);
    return 0;
  } catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

int sudare_set_led_rect(int x, int y, int z, int rgb) {
  try {
    sudare->set_led_rect(x, y, z, rgb);
    return 0;
  } catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

int sudare_set_led_polar(int a, int r, int h, int rgb) {
  try {
    sudare->set_led_polar(a, r, h, rgb);
    return 0;
  } catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

int sudare_clear(void) {
  try {
    sudare->clear();
    return 0;
  } catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

int sudare_send(void) {
  try {
    sudare->send();
    return 0;
  } catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

int sudare_sleep(int ms) {
  try {
    sudare->sleep(ms);
    return 0;
  } catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
