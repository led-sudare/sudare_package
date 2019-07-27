#pragma once

#include <cstddef>  // size_t
#include <vector>
#include "rgb.hpp"

#ifndef POLAR_ANGLE_RESOLUTION
#define POLAR_ANGLE_RESOLUTION 6
#endif

#ifndef POLAR_ANGLES
#define POLAR_ANGLES 360 / POLAR_ANGLE_RESOLUTION
#endif

#ifndef POLAR_RADIUS
#define POLAR_RADIUS 15
#endif

#ifndef POLAR_HEIGHT
#define POLAR_HEIGHT 100
#endif

#ifndef SUDARE_ANGLE_RESOLUTION
#define SUDARE_ANGLE_RESOLUTION POLAR_ANGLE_RESOLUTION
#endif

#ifndef SUDARE_ANGLES
#define SUDARE_ANGLES POLAR_ANGLES
#endif

#ifndef SUDARE_RADIUS
#define SUDARE_RADIUS 15
#endif

#ifndef SUDARE_HEIGHT
#define SUDARE_HEIGHT 100
#endif

namespace sudare {
class polar {
  int m_angles;
  int m_radius;
  int m_height;
  std::vector<char> m;
  /** 座標インデックス */
  int index(int a, int r, int h) const;
  /** 座標が内部にある判定 */
  bool contains(int a, int r, int h) const;

 public:
  /**
   * @param[in] angles 角度数
   * @param[in] radius 半径
   * @param[in] height 高さ
   */
  polar(int angles, int radius, int height);
  /** 消去 */
  void clear();
  /** 色設定 */
  void set(int a, int r, int h, int color);
  /** 色設定 */
  void set(int a, int r, int h, rgb const& color);
  /** 指定位置の色取得 */
  char* data(int a, int r, int h);
  /** 指定位置の色取得 */
  char const* data(int a, int r, int h) const;
  /** 先頭位置の色取得 */
  char* data();
  /** 先頭位置の色取得 */
  char const* data() const;
  /** バッファサイズ */
  size_t size() const;
  /** 角度数 */
  int angles() const;
  /** 半径 */
  int radius() const;
  /** 高さ */
  int height() const;
};
}  // namespace sudare
