#include "sudare.h"
#include <array>
#include <iostream>
#include <memory>
#include <thread>
#include "converter.hpp"
#include "spi.h"

#define POLAR_HEIGHT 100
#define POLAR_ANGLE_RESOLUTION 6
#define POLAR_ANGLES 360 / POLAR_ANGLE_RESOLUTION
#define POLAR_RADIUS 15

namespace {
std::shared_ptr<sudare::rectangular> s_rect;
std::shared_ptr<sudare::polar> s_polar;
std::shared_ptr<sudare::converter> s_conv;
std::shared_ptr<sudare::spi> s_spi;
}  // namespace

int InitSdk(int width, int height, int depth, int clock_MHz) {
  try {
    s_rect = std::make_shared<sudare::rectangular>(width, height, depth);
    s_polar = std::make_shared<sudare::polar>(POLAR_ANGLES, POLAR_RADIUS,
                                              POLAR_HEIGHT);
    s_conv = std::make_shared<sudare::bilinear_converter>(*s_rect, *s_polar);
    s_spi = std::make_shared<sudare::spi>(10 * 1000 * 1000);
    return 0;
  } catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

void SetLed(int x, int y, int z, int rgb) { s_rect->set(x, y, z, rgb); }

void Clear() { s_rect->clear(); }

void Show() {
  (*s_conv)();
  const int dlen = 3000;
  for (int a = 0; a < POLAR_ANGLES; ++a) {
    std::array<char, dlen + 4> pkt{2, 0, 0};  // WR, AD0, AD1
    pkt.back() = a;
    char const *begin = s_polar->data(a, 0, 0);
    char const *end = begin + dlen;
    std::copy(begin, end, pkt.data() + 3);
    s_spi->write(pkt.data(), pkt.size(),
                 0);  // BUF, LEN, CS
  }
}

void Wait(int ms) {
  static auto sLastWaitTime = std::chrono::system_clock::now();
  auto now = std::chrono::system_clock::now();
  int diff = static_cast<int>(
      std::chrono::duration_cast<std::chrono::milliseconds>(now - sLastWaitTime)
          .count());
  ms = std::max(1, ms - diff);
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
  sLastWaitTime = std::chrono::system_clock::now();
}

void DrawAll(uint8_t red, uint8_t green, uint8_t blue) {
  sudare::rgb color(red, green, blue);
  for (char a = 0; a < 60; ++a) {
    std::array<char, 3004> pkt{2, 0, 0};  // WR, AD0, AD1
    pkt.back() = a;
    char *p = pkt.data() + 3;
    for (int r = 1; r <= 15; ++r)
      for (int y = 0; y < 100; ++y, p += 2) color.to565(p);
    s_spi->write(pkt.data(), pkt.size(), 0);  // buf, len, cs
  }
}
