#include "converter.h"
#include <cmath>
#include <iostream>
#include "time_meter.hpp"

namespace sudare {
void bilinear_converter::mod_sincos(polar const& po) {
  if (m_sin.size() - po.angles() == 0 && m_cos.size() == m_sin.size()) return;
  m_sin.resize(po.angles());
  m_cos.resize(po.angles());
  for (size_t i = 0; i < m_sin.size(); ++i) {
    double theta = M_PI / 180 * i * (360 / m_sin.size());
    m_sin[i] = std::sin(theta);
    m_cos[i] = std::cos(theta);
  }
}
void bilinear_converter::operator()(rectangular const& rect, polar& po) const {
  const_cast<bilinear_converter*>(this)->mod_sincos(po);
  time_meter tm("bilinear_converter");
  const double rx = (rect.width() - 1.0) / (po.radius() * 2);
  const double rz = (rect.depth() - 1.0) / (po.radius() * 2);
  const double ry = (rect.height() - 1.0) / (po.height() - 1);
  for (int a = 0; a < po.angles(); ++a) {
    for (int r = 1; r <= po.radius(); ++r) {
      double ax = (po.radius() + m_cos[a] * r) * rx;
      double az = (po.radius() + m_sin[a] * r) * rz;
      for (int y = 0; y < po.height(); ++y) {
        double ay = y * ry;
        rgb color = rect.getBL(ax, ay, az);
        po.set(a, r, y, color);
      }
    }
  }
}

void nearest_neighbor_converter::mod_sincos(polar const& po) {
  if (m_sin.size() - po.angles() == 0 && m_cos.size() == m_sin.size()) return;
  m_sin.resize(po.angles());
  m_cos.resize(po.angles());
  for (size_t i = 0; i < m_sin.size(); ++i) {
    double theta = M_PI / 180 * i * (360 / m_sin.size());
    m_sin[i] = std::sin(theta);
    m_cos[i] = std::cos(theta);
  }
}
void nearest_neighbor_converter::operator()(rectangular const& rect,
                                            polar& po) const {
  const_cast<nearest_neighbor_converter*>(this)->mod_sincos(po);
  time_meter tm("nearest_neighbor_converter");
  const double rx = (rect.width() - 1.0) / (po.radius() * 2);
  const double rz = (rect.depth() - 1.0) / (po.radius() * 2);
  const double ry = (rect.height() - 1.0) / (po.height() - 1);
  for (int a = 0; a < po.angles(); ++a) {
    for (int r = 1; r <= po.radius(); ++r) {
      double ax = (po.radius() + m_cos[a] * r) * rx;
      double az = (po.radius() + m_sin[a] * r) * rz;
      for (int y = 0; y < po.height(); ++y) {
        double ay = y * ry;
        rgb color = rect.getNN(ax, ay, az);
        po.set(a, r, y, color);
      }
    }
  }
}
}  // namespace sudare
