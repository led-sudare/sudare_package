#include "sudare.h"
#include <array>
#include <iostream>
#include <memory>
#include <thread>
#include "converter.h"
#include "publisher.h"

namespace {
std::shared_ptr<sudare::rectangular> s_rect;
std::shared_ptr<sudare::polar> s_polar;
std::shared_ptr<sudare::converter> s_conv;
std::shared_ptr<sudare::publisher> s_publisher;
}  // namespace

int InitSdk(int width, int height, int depth, int clock_MHz) {
  try {
    s_rect = std::make_shared<sudare::rectangular>(width, height, depth);
    s_polar = std::make_shared<sudare::polar>(POLAR_ANGLES, POLAR_RADIUS,
                                              POLAR_HEIGHT);
    s_conv = std::make_shared<sudare::bilinear_converter>(*s_rect, *s_polar);
    s_publisher =
        std::make_shared<sudare::spi_publisher>(clock_MHz * 1000 * 1000);
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
  (*s_publisher)(*s_polar);
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
  sudare::rgb const color(red, green, blue);
  for (int a = 0; a < s_polar->angles(); ++a)
    for (int r = 0; r < s_polar->radius(); ++r)
      for (int h = 0; h < s_polar->height(); ++h) s_polar->set(a, r, h, color);
  (*s_publisher)(*s_polar);
}

void DrawRGBLines(void) {
  sudare::rgb const red(0xFF, 0x00, 0x00);
  sudare::rgb const green(0x00, 0xFF, 0x00);
  sudare::rgb const blue(0x00, 0x00, 0xFF);
  sudare::rgb const colors[] = {red, green, blue};
  for (int a = 0; a < s_polar->angles(); ++a) {
    for (int r = 0; r < s_polar->radius(); ++r) {
      auto color = colors[r % 3];
      for (int h = 0; h < s_polar->height(); ++h) {
        s_polar->set(a, r, h, color);
      }
    }
  }
  (*s_publisher)(*s_polar);
}
