#include "publisher.h"
#include <array>
#include <iostream>
#include <vector>
#include "polar.h"
#include "time_meter.hpp"

namespace sudare {
zmq_publisher::zmq_publisher(void* context, const char* dst)
    : m_client(context, dst) {}

int zmq_publisher::operator()(const char* data, size_t size) {
  time_meter tm("zmq_publisher");
  return m_client.send(data, size);
}

spi_publisher::spi_publisher(int clock) : m_spi(clock) {}
int spi_publisher::operator()(const char* data, size_t size) {
  time_meter tm("spi_publisher");
  const int angles = 60;
  const int dlen = 3000;
  if (size != angles * dlen) {
    std::cerr << "invalid size : " << size << std::endl;
    throw std::invalid_argument("spi_publisher::operator()");
  }
  int total = 0;
  for (int a = 0; a < angles; ++a) {
    std::array<char, dlen + 4> pkt = {2, 0, 0};  // WRITE, AD0, AD1
    pkt.back() = static_cast<char>(a);
    const char* begin = data + a * dlen;
    const char* end = begin + dlen;
    std::copy(begin, end, pkt.data() + 3);
    total += static_cast<int>(m_spi.write(pkt.data(), pkt.size(), 0));
  }
  std::cout << "write spi : " << total << "bytes" << std::endl;
  return total;
}

namespace {
rgbd average(polar const& src, int a, int r0, int r1, int y0, int y1) {
  rgbd v;
  for (int r = r0; r < r1; ++r) {
    for (int y = y0; y < y1; ++y) {
      // TODO: 足す
    }
  }
  return v;  // TODO: 割る
}
}  // namespace
spi_mini_publisher::spi_mini_publisher(int clock) : m_spi(clock) {}
int spi_mini_publisher::operator()(const char* data, size_t size) {
  time_meter tm("spi_mini_publisher");
  const int angles = 60;
  const int dlen = 3000;
  if (size != angles * dlen) {
    std::cerr << "invalid size : " << size << std::endl;
    throw std::invalid_argument("spi_mini_publisher::operator()");
  }
  polar po(SUDARE_ANGLES, SUDARE_RADIUS, SUDARE_HEIGHT, data);
  int total = 0;
  for (int a = 0; a < angles; ++a) {
    std::array<char, dlen + 4> pkt = {2, 0, 0};  // WRITE, AD0, AD1
    pkt.back() = static_cast<char>(a);
    for (int r0 = 0; r0 < 5; ++r0) {
      for (int y0 = 0; y0 < 10; ++y0) {
        rgbd v = average(po, a, r0, r0 + 3, y0, y0 + 10);
        int index = ((a * SUDARE_RADIUS + (r0 + 10)) * SUDARE_HEIGHT + y0) * 2;
        v.to565(pkt.data() + 3 + index);
      }
    }
    total += static_cast<int>(m_spi.write(pkt.data(), pkt.size(), 0));
  }
  std::cout << "write spi : " << total << "bytes" << std::endl;
  return total;
}
}  // namespace sudare

/* SPI通信量を間引く処理。FPGAの都合で使用不可となったが将来的に復活するかもしれないし、何より作るのが大変だったからとっておく。
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
}
 */
