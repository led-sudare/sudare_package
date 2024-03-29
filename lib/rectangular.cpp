#include "rectangular.h"
#include <cmath>
#include <iostream>

namespace sudare {
namespace {
/** 2点間の中間色を得る */
template <typename T>
rgbd calc(rgb_<T> const& c0, double r, rgb_<T> const& c1) {
  return rgbd(c0.getR() * (1 - r) + c1.getR() * r,
              c0.getG() * (1 - r) + c1.getG() * r,
              c0.getB() * (1 - r) + c1.getB() * r);
}
}  // namespace

/** 座標が内部にある判定 */
bool rectangular::contains(int x, int y, int z) const {
  return 0 <= x && x < m_w && 0 <= y && y < m_h && 0 <= z && z < m_d;
}
/** 座標インデックス */
int rectangular::index(int x, int y, int z) const {
  if (contains(x, y, z)) return x + (y + z * m_h) * m_w;
  std::cerr << "invalid index " << x << ", " << y << ", " << z << std::endl;
  throw std::out_of_range("rectangular::index");
}

rectangular::rectangular(int w, int h, int d) {
  m_w = w;
  m_h = h;
  m_d = d;
  m.resize(m_w * m_h * m_d);
}
/** 幅取得 */
int rectangular::width() const { return m_w; }
/** 高さ取得 */
int rectangular::height() const { return m_h; }
/** 奥行き取得 */
int rectangular::depth() const { return m_d; }
/** 色設定 */
void rectangular::set(int x, int y, int z, int color) {
  if (contains(x, y, z)) m[index(x, y, z)] = rgb(color);
}
/** 色取得 */
rgb rectangular::get(int x, int y, int z) const { return m[index(x, y, z)]; }
/** 3D LEDパケットからの一括入力 */
void rectangular::set_from_3d_led_pkt(char const* p) {
  const int W = 16;
  const int H = 32;
  const int D = 8;
  if (m_w != W || m_h != H || m_d != D)
    throw std::invalid_argument(
        "size should be following values. W = 16, H = 32, D = 8");
  for (int x = 0; x < W; ++x)
    for (int y = 0; y < H; ++y)
      for (int z = 0; z < D; ++z, p += 2) m[index(x, y, z)] = rgb(p);
}
/** 全座標クリア */
void rectangular::clear() { std::fill(m.begin(), m.end(), rgb()); }
/** ニアレストネイバー */
rgb rectangular::getNN(double x, double y, double z) const {
  int ax = static_cast<int>(std::round(x));
  int ay = static_cast<int>(std::round(y));
  int az = static_cast<int>(std::round(z));
  return m[index(ax, ay, az)];
}
/** バイリニア */
rgb rectangular::getBL(double x, double y, double z) const {
  // 点(x, y, z)を含む、1x1x1サイズの立方体を作る。
  int x0 = static_cast<int>(std::floor(x));
  int x1 = static_cast<int>(std::ceil(x));
  int y0 = static_cast<int>(std::floor(y));
  int y1 = static_cast<int>(std::ceil(y));
  int z0 = static_cast<int>(std::floor(z));
  int z1 = static_cast<int>(std::ceil(z));
  // 立方体の8点をX方向に圧縮して4点にする
  double rx = x - x0;
  rgbd c00 = calc(get(x0, y0, z0), rx, get(x1, y0, z0));
  rgbd c01 = calc(get(x0, y0, z1), rx, get(x1, y0, z1));
  rgbd c10 = calc(get(x0, y1, z0), rx, get(x1, y1, z0));
  rgbd c11 = calc(get(x0, y1, z1), rx, get(x1, y1, z1));
  // 同様にY方向に圧縮して2点にする
  double ry = y - y0;
  rgbd d0 = calc(c00, ry, c10);
  rgbd d1 = calc(c01, ry, c11);
  // 最後にZ方向に圧縮する
  rgbd res = calc(d0, z - z0, d1);
  return rgb(static_cast<uint8_t>(res.getR()), static_cast<uint8_t>(res.getG()),
             static_cast<uint8_t>(res.getB()));
}
}  // namespace sudare
