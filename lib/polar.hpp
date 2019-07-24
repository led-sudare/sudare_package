#pragma once

#include <vector>
#include "rgb.hpp"

class polar {
  int m_angles;
  int m_radius;
  int m_height;
  std::vector<uint8_t> m;
  int index(int a, int r, int h) const {
    a %= m_angles;
    return ((a * m_radius + r) * m_height + h) * 2;
  }
  bool contains(int a, int r, int h) const {
    a %= m_angles;
    return 0 <= a && a < m_angles && 0 <= r && r < m_radius && 0 <= h &&
           h < m_height;
  }

 public:
  polar(int angles, int radius, int height)
      : m_angles(angles),
        m_radius(radius),
        m_height(height),
        m(angles * radius * height * 2) {}
  void clear() { std::fill(m.begin(), m.end(), 0); }
  void set(int a, int r, int h, int rgb) { set(a, r, h, rgb(rgb)); }
  void set(int a, int r, int h, rgb rgb) {
    if (contains(a, r, h)) rgb.to565(data(a, r, h));
  }
  uint8_t* data(int a, int r, int h) { return m.data() + index(a, r, h); }
  uint8_t const* data(int a, int r, int h) const {
    return m.data() + index(a, r, h);
  }
  uint8_t const* data() const { return m.data(); }
  uint8_t* data() { return m.data(); }
  size_t size() const { return m.size(); }
  int angles() const { return m_angles; }
  int radius() const { return m_radius; }
  int height() const { return m_height; }
};
