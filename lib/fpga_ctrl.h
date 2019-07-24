#pragma once

#include <array>
#include "spi.h"

#define SUDARE_WIDTH 30
#define SUDARE_HEIGHT 100
#define SUDARE_ANGLE_RESOLUTION 6
#define SUDARE_ANGLES 360 / SUDARE_ANGLE_RESOLUTION
#define SUDARE_PKT_SIZE SUDARE_WIDTH *SUDARE_HEIGHT *SUDARE_ANGLES

namespace sudare {
class fpga_ctrl {
  mutable std::array<char, SUDARE_PKT_SIZE> m_cache;
  mutable int m_total_sent;
  mutable int m_sent_count;
  spi m_spi;
  void spi_write(const char *p, int size, int cs) const;
  void spi_log() const;
  /**通常の通信モード */
  void write0(const char *p) const;
  /**前回と異なっていたLEDのみを転送する*/
  void write1(const char *p) const;
  /**前回と異なっていた面のみを転送する*/
  void write2(const char *p) const;

 public:
  explicit fpga_ctrl(uint8_t clock_MHz);
  virtual ~fpga_ctrl();
  /**@param[in] p SUDARE PACKET begin address*/
  void write(const char *p) const { write2(p); }
};
}  // namespace sudare
