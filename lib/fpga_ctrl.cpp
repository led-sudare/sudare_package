#include "fpga_ctrl.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>  // close, usleep
#include <cstring>   // strerror
#include <iostream>
#include <sstream>  // std::stringstream, errno, std::runtime_error
#ifdef ENABLE_SPI
#include <linux/spi/spidev.h>
#endif  // ENABLE_SPI

namespace {
const int hlen = 3;
const int dlen = SUDARE_WIDTH * SUDARE_HEIGHT;
const int cs = 0;
const int angles = 360 / ANGLE_RESOLUTION;
}  // namespace

fpga_ctrl::fpga_ctrl(uint8_t clock_MHz)
    : m_total_sent(0), m_sent_count(0), m_spi(clock_MHz * 1000 * 1000) {
  std::fill(m_cache.begin(), m_cache.end(), 0);
}

fpga_ctrl::~fpga_ctrl() {}

void fpga_ctrl::spi_write(const char *p, int size, int cs) const {
  m_spi.write(p, size, cs);
  m_total_sent += size;
  m_sent_count++;
}

void fpga_ctrl::spi_log() const {
  float percent = 100.0f * m_total_sent / ((hlen + dlen + 1) * angles);
  printf("SPI TX SIZE = %d (%.1f%%) TX COUNT = %d\n", m_total_sent, percent,
         m_sent_count);
  m_total_sent = 0;
  m_sent_count = 0;
}

void fpga_ctrl::write0(const char *p) const {
  std::array<char, hlen + dlen + 1> pkt = {2, 0, 0};  // WRITE, AD0, AD1
  for (int a = 0; a < angles; ++a) {
    const char *d0 = p + a * dlen;
    std::copy(d0, d0 + dlen, pkt.data() + hlen);
    pkt.back() = static_cast<char>(a);
    spi_write(pkt.data(), pkt.size(), cs);
  }
  spi_log();
}

void fpga_ctrl::write1(const char *p) const {
  std::array<char, dlen + hlen> pkt0 = {2, 0, 0};  // WRITE, AD0, AD1
  std::array<char, 1 + hlen> pkt1 = {
      2, static_cast<char>(dlen >> 8),
      static_cast<char>(dlen & 0xFF)};  // WRITE, AD0, AD1
  for (int a = 0; a < angles; ++a) {
    const char *begin = p + a * dlen;
    const char *end = begin + dlen;
    const char *cache = m_cache.data() + a * dlen;
    const char *mark = nullptr;
    for (auto it = begin; it != end; ++it, ++cache) {
      const bool match = *it == *cache;
      const bool last = it == end - 1;
      if (!match && mark == nullptr) mark = it;
      if (last || match) {
        if (mark == nullptr) continue;
        int size = static_cast<int>(it - mark) + 1;
        if (size < 32 && !last)
          continue;  // ここで適当に間引かないとかえって遅くなる
        std::copy(it, it + size, pkt0.data() + hlen);
        spi_write(pkt0.data(), hlen + size, cs);
        mark = nullptr;
      }
    }
    pkt1[hlen] = static_cast<char>(a);
    spi_write(pkt1.data(), pkt1.size(), cs);
  }
  std::copy(p, p + m_cache.size(), m_cache.begin());
  spi_log();
}

void fpga_ctrl::write2(const char *p) const {
  std::array<char, hlen + dlen + 1> pkt = {2, 0, 0};  // WRITE, AD0, AD1
  for (int a = 0; a < angles; ++a) {
    const char *begin = p + a * dlen;
    const char *end = begin + dlen;
    char *cache = m_cache.data() + a * dlen;
    char *const cbegin = cache;
    int th = static_cast<int>(dlen * 0.05);  // 適当に閾値を設定する
    int count = 0;
    for (auto it = begin; it != end; ++it, ++cache) {
      if (*it != *cache) ++count;
      if (count < th) continue;
      std::copy(begin, end, pkt.begin() + hlen);
      pkt.back() = static_cast<char>(a);
      spi_write(pkt.data(), pkt.size(), cs);
      std::copy(begin, end, cbegin);
      break;
    }
  }
  spi_log();
}
