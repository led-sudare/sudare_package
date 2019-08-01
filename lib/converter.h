#pragma once

#include "polar.h"
#include "rectangular.h"

namespace sudare {
class converter {
 public:
  virtual ~converter() {}
  virtual void operator()(rectangular const& rect, polar& polar) const = 0;
};

/** 直交座標から極座標へ変換する。補間はバイリニア。 */
class bilinear_converter : public converter {
  std::vector<double> m_sin;
  std::vector<double> m_cos;
  void mod_sincos(polar const& po);

 public:
  ~bilinear_converter() {}
  void operator()(rectangular const& rect, polar& po) const;
};

/** 直交座標から極座標へ変換する。補間はニアレストネイバー。 */
class nearest_neighbor_converter : public converter {
  std::vector<double> m_sin;
  std::vector<double> m_cos;
  void mod_sincos(polar const& po);

 public:
  ~nearest_neighbor_converter() {}
  void operator()(rectangular const& rect, polar& po) const;
};
}  // namespace sudare
