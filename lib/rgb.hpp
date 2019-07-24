#pragma once

#include <cstdint>  // uint8_t

namespace sudare {
template <typename T>
class rgb_ {
  T r;
  T g;
  T b;

  template <int RMASK, int GMASK0, int GMASK1, int BMASK>
  void toNNN(uint8_t* buf) const {
    uint8_t r0 = static_cast<uint8_t>(r);
    uint8_t g0 = static_cast<uint8_t>(g);
    uint8_t b0 = static_cast<uint8_t>(b);
    buf[0] = (r0 & RMASK) | ((g0 & GMASK0) >> 5);
    buf[1] = ((g0 & GMASK1) << 3) | ((b0 & BMASK) >> 3);
  }

 public:
  rgb_() : r(0), g(0), b(0) {}
  rgb_(T red, T green, T blue) : r(red), g(green), b(blue) {}
  explicit rgb_(int rgb)
      : r((rgb & 0xFF0000) >> 16),
        g((rgb & 0x00FF00) >> 8),
        b((rgb & 0x0000FF) >> 0) {}
  explicit rgb_(uint8_t const* p565)
      : r(p565[0] & 0xF8),
        g(((p565[0] & 0x07) << 5) + ((p565[1] & 0xE0) >> 3)),
        b((p565[1] & 0x1F) << 3) {}
  void to565(uint8_t* buf) const { return toNNN<0xF8, 0xE0, 0x1C, 0xF8>(buf); }
  void to444(uint8_t* buf) const { return toNNN<0xF0, 0xE0, 0x10, 0xF0>(buf); }
  void to333(uint8_t* buf) const { return toNNN<0xE0, 0xE0, 0x00, 0xE0>(buf); }
  T getR() const { return r; }
  T getG() const { return g; }
  T getB() const { return b; }
};

typedef rgb_<uint8_t> rgb;
typedef rgb_<double> rgbd;
}  // namespace sudare
