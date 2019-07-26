#pragma once

#include <array>
#include "spi.h"
#include "zmq_utils.h"

namespace sudare {
class publisher {
 public:
  virtual ~publisher() {}
  virtual int operator()(const char* data, size_t size) = 0;
};
class zmq_publisher : public publisher {
  sudare::zmq_client m_client;

 public:
  zmq_publisher(void* context, const char* dst) : m_client(context, dst) {}
  ~zmq_publisher() {}
  int operator()(const char* data, size_t size) {
    return m_client.send(data, size);
  }
};
class spi_publisher : public publisher {
  sudare::spi m_spi;

 public:
  explicit spi_publisher(int clock) : m_spi(clock) {}
  int operator()(const char* data, size_t size) {
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
    return total;
  }
};
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
