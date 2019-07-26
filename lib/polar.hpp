#pragma once

#include <vector>
#include "rgb.hpp"

namespace sudare {
class polar {
  int m_angles;
  int m_radius;
  int m_height;
  std::vector<char> m;
  /** 座標インデックス */
  int index(int a, int r, int h) const {
    a %= m_angles;
    if (contains(a, r, h)) return ((a * m_radius + r) * m_height + h) * 2;
    std::cerr << "invalid index " << a << ", " << r << ", " << h << std::endl;
    throw std::out_of_range("polar::index");
  }
  /** 座標が内部にある判定 */
  bool contains(int a, int r, int h) const {
    // aは何周してもよいので確認しない
    return 0 <= r && r < m_radius && 0 <= h && h < m_height;
  }

 public:
  polar(int angles, int radius, int height)
      : m_angles(angles),
        m_radius(radius),
        m_height(height),
        m(angles * radius * height * 2) {}
  void clear() { std::fill(m.begin(), m.end(), 0); }
  void set(int a, int r, int h, int color) { set(a, r, h, rgb(color)); }
  void set(int a, int r, int h, rgb color) {
    if (contains(a, r, h)) color.to565(data(a, r, h));
  }
  char* data(int a, int r, int h) { return m.data() + index(a, r, h); }
  char const* data(int a, int r, int h) const {
    return m.data() + index(a, r, h);
  }
  char const* data() const { return m.data(); }
  char* data() { return m.data(); }
  size_t size() const { return m.size(); }
  int angles() const { return m_angles; }
  int radius() const { return m_radius; }
  int height() const { return m_height; }
};
}  // namespace sudare
