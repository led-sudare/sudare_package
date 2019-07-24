#pragma once

#include <cmath>
#include <vector>
#include "rgb.hpp"

namespace sudare {
class rectangular {
  std::vector<rgb> m;
  size_t m_w;
  size_t m_h;
  size_t m_d;
  /** 座標が内部にある判定 */
  bool contains(int x, int y, int z) const {
    return 0 <= x && x < static_cast<int>(m_w) && 0 <= y &&
           y < static_cast<int>(m_h) && 0 <= z && z < static_cast<int>(m_d);
  }
  /** 座標アドレス */
  size_t address(int x, int y, int z) const {
    if (!contains(x, y, z)) throw std::out_of_range("rectangular::address");
    return static_cast<size_t>(x + (y + z * m_h) * m_w);
  }

  /** 2点間の中間色を得る */
  template <typename T>
  static rgbd calc(rgb_<T> const& c0, double r, rgb_<T> const& c1) {
    return rgbd(c0.getR() * (1 - r) + c1.getR() * r,
                c0.getG() * (1 - r) + c1.getG() * r,
                c0.getB() * (1 - r) + c1.getB() * r);
  }

 public:
  rectangular(size_t w, size_t h, size_t d) {
    m_w = w;
    m_h = h;
    m_d = d;
    m.resize(m_w * m_h * m_d);
  }
  /** 幅取得 */
  size_t getW() const { return m_w; }
  /** 高さ取得 */
  size_t getH() const { return m_h; }
  /** 奥行き取得 */
  size_t getD() const { return m_d; }
  /** 色設定 */
  void set(int x, int y, int z, int rgb) {
    if (contains(x, y, z)) m[address(x, y, z)] = rgb(rgb);
  }
  /** 3D LEDパケットからの一括入力 */
  void from3DLED(uint8_t const* p) {
    const int W = 16;
    const int H = 32;
    const int D = 8;
    if (m_w != W || m_h != H || m_d != D)
      throw std::invalid_argument(
          "size should be following values. W = 16, H = 32, D = 8");
    for (int x = 0; x < W; ++x)
      for (int y = 0; y < H; ++y)
        for (int z = 0; z < D; ++z, p += 2) m[address(x, y, z)] = rgb(p);
  }
  /** 全座標クリア */
  void clear() { std::fill(m.begin(), m.end(), rgb()); }
  /** ニアレストネイバー */
  rgb getNN(double x, double y, double z) const {
    int ax = static_cast<int>(std::round(x));
    int ay = static_cast<int>(std::round(y));
    int az = static_cast<int>(std::round(z));
    return m[address(ax, ay, az)];
  }
  /** バイリニア */
  rgb getBL(double x, double y, double z) const {
    // 点(x, y, z)を含む、1x1x1サイズの立方体を作る。
    int x0 = static_cast<int>(std::floor(x));
    int x1 = static_cast<int>(std::ceil(x));
    int y0 = static_cast<int>(std::floor(y));
    int y1 = static_cast<int>(std::ceil(y));
    int z0 = static_cast<int>(std::floor(z));
    int z1 = static_cast<int>(std::ceil(z));
    // 指定位置の色取得
    auto rgb = [this](int x, int y, int z) { return m[address(x, y, z)]; };
    // 立方体の8点をX方向に圧縮して4点にする
    double rx = x - x0;
    rgbd c00 = calc(rgb(x0, y0, z0), rx, rgb(x1, y0, z0));
    rgbd c01 = calc(rgb(x0, y0, z1), rx, rgb(x1, y0, z1));
    rgbd c10 = calc(rgb(x0, y1, z0), rx, rgb(x1, y1, z0));
    rgbd c11 = calc(rgb(x0, y1, z1), rx, rgb(x1, y1, z1));
    // 同様にY方向に圧縮して2点にする
    double ry = y - y0;
    rgbd d0 = calc(c00, ry, c10);
    rgbd d1 = calc(c01, ry, c11);
    // 最後にZ方向に圧縮する
    rgbd res = calc(d0, z - z0, d1);
    return rgb(static_cast<uint8_t>(res.getR()),
               static_cast<uint8_t>(res.getG()),
               static_cast<uint8_t>(res.getB()));
  }
};
}  // namespace sudare
