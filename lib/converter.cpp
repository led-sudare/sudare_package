#include "converter.h"
#include <cmath>
#include <iostream>

namespace sudare {
converter::converter(rectangular const& rect, polar& polar)
    : m_rect(rect), m_polar(polar) {}

bilinear_converter::bilinear_converter(rectangular const& rect, polar& polar)
    : converter(rect, polar), m_sin(polar.angles()), m_cos(polar.angles()) {
  for (size_t i = 0; i < m_sin.size(); ++i) {
    double theta = M_PI / 180 * i * (360 / polar.angles());
    m_sin[i] = std::sin(theta);
    m_cos[i] = std::cos(theta);
  }
}

void bilinear_converter::operator()() {
  const double rx = (m_rect.getW() - 1.0) / (m_polar.radius() * 2);
  const double rz = (m_rect.getD() - 1.0) / (m_polar.radius() * 2);
  const double ry = (m_rect.getH() - 1.0) / (m_polar.height() - 1);
  for (int a = 0; a < m_polar.angles(); ++a) {
    for (int r = 1; r <= m_polar.radius(); ++r) {
      double ax = (m_polar.radius() + m_cos[a] * r) * rx;
      double az = (m_polar.radius() + m_sin[a] * r) * rz;
      for (int y = 0; y < m_polar.height(); ++y) {
        double ay = y * ry;
        rgb color = m_rect.getBL(ax, ay, az);
        m_polar.set(a, r, y, color);
      }
    }
  }
}

nearest_neighbor_converter::nearest_neighbor_converter(rectangular const& rect,
                                                       polar& polar)
    : converter(rect, polar), m_sin(polar.angles()), m_cos(polar.angles()) {
  for (size_t i = 0; i < m_sin.size(); ++i) {
    double theta = M_PI / 180 * i * (360 / polar.angles());
    m_sin[i] = std::sin(theta);
    m_cos[i] = std::cos(theta);
  }
}

void nearest_neighbor_converter::operator()() {
  const double rx = (m_rect.getW() - 1.0) / (m_polar.radius() * 2);
  const double rz = (m_rect.getD() - 1.0) / (m_polar.radius() * 2);
  const double ry = (m_rect.getH() - 1.0) / (m_polar.height() - 1);
  for (int a = 0; a < m_polar.angles(); ++a) {
    for (int r = 1; r <= m_polar.radius(); ++r) {
      double ax = (m_polar.radius() + m_cos[a] * r) * rx;
      double az = (m_polar.radius() + m_sin[a] * r) * rz;
      for (int y = 0; y < m_polar.height(); ++y) {
        double ay = y * ry;
        rgb color = m_rect.getNN(ax, ay, az);
        m_polar.set(a, r, y, color);
      }
    }
  }
}
}  // namespace sudare
