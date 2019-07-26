#pragma once

#include "polar.h"
#include "rectangular.h"

namespace sudare {
class converter {
 protected:
  rectangular const& m_rect;
  polar& m_polar;

 public:
  converter(rectangular const& rect, polar& polar);
  virtual ~converter() {}
  virtual void operator()() = 0;
};

/** 直交座標から極座標へ変換する。補間はバイリニア。 */
class bilinear_converter : public converter {
  std::vector<double> m_sin;
  std::vector<double> m_cos;

 public:
  bilinear_converter(rectangular const& rect, polar& polar);
  ~bilinear_converter() {}
  void operator()();
};

/** 直交座標から極座標へ変換する。補間はニアレストネイバー。 */
class nearest_neighbor_converter : public converter {
  std::vector<double> m_sin;
  std::vector<double> m_cos;

 public:
  nearest_neighbor_converter(rectangular const& rect, polar& polar);
  ~nearest_neighbor_converter() {}
  void operator()();
};
}  // namespace sudare
