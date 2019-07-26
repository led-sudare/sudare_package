#pragma once

#include <vector>
#include "rgb.hpp"

#ifndef RECTANGULAR_WIDTH
#define RECTANGULAR_WIDTH 30
#endif

#ifndef RECTANGULAR_HEIGHT
#define RECTANGULAR_HEIGHT 100
#endif

#ifndef RECTANGULAR_DEPTH
#define RECTANGULAR_DEPTH 30
#endif

#ifndef CUBE_WIDTH
#define CUBE_WIDTH 16
#endif

#ifndef CUBE_HEIGHT
#define CUBE_HEIGHT 32
#endif

#ifndef CUBE_DEPTH
#define CUBE_DEPTH 8
#endif

namespace sudare {
class rectangular {
  std::vector<rgb> m;
  int m_w;
  int m_h;
  int m_d;
  /** 座標が内部にある判定 */
  bool contains(int x, int y, int z) const;
  /** 座標インデックス */
  int index(int x, int y, int z) const;

 public:
  rectangular(int w, int h, int d);
  /** 幅取得 */
  int width() const;
  /** 高さ取得 */
  int height() const;
  /** 奥行き取得 */
  int depth() const;
  /** 色設定 */
  void set(int x, int y, int z, int color);
  /** 色取得 */
  rgb get(int x, int y, int z) const;
  /** 3D LEDパケットからの一括入力 */
  void set_from_3d_led_pkt(char const* p);
  /** 全座標クリア */
  void clear();
  /** ニアレストネイバー */
  rgb getNN(double x, double y, double z) const;
  /** バイリニア */
  rgb getBL(double x, double y, double z) const;
};
}  // namespace sudare
