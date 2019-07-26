#include "polar.h"
#include <iostream>

namespace sudare {
int polar::index(int a, int r, int h) const {
  a %= m_angles;
  if (contains(a, r, h)) return ((a * m_radius + r) * m_height + h) * 2;
  std::cerr << "invalid index " << a << ", " << r << ", " << h << std::endl;
  throw std::out_of_range("polar::index");
}

bool polar::contains(int a, int r, int h) const {
  // aは何周してもよいので確認しない
  static_cast<void>(a);  // unused
  return 0 <= r && r < m_radius && 0 <= h && h < m_height;
}

polar::polar(int angles, int radius, int height)
    : m_angles(angles),
      m_radius(radius),
      m_height(height),
      m(angles * radius * height * 2) {}

void polar::clear() { std::fill(m.begin(), m.end(), 0); }

void polar::set(int a, int r, int h, int color) { set(a, r, h, rgb(color)); }

void polar::set(int a, int r, int h, rgb color) {
  if (contains(a, r, h)) color.to565(data(a, r, h));
}

char* polar::data(int a, int r, int h) { return m.data() + index(a, r, h); }

char const* polar::data(int a, int r, int h) const {
  return m.data() + index(a, r, h);
}

char const* polar::data() const { return m.data(); }

char* polar::data() { return m.data(); }

size_t polar::size() const { return m.size(); }

int polar::angles() const { return m_angles; }

int polar::radius() const { return m_radius; }

int polar::height() const { return m_height; }
}  // namespace sudare
