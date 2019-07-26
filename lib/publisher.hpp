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
    int total = 0;
    for (int a = 0; a < 60; ++a) {
      std::array<char, 3004> pkt = {2, 0, 0};  // WRITE, AD0, AD1
      pkt.back() = static_cast<char>(a);
      std::copy(data, data + size, pkt.data() + 3);
      total += static_cast<int>(m_spi.write(pkt.data(), pkt.size(), 0));
    }
    return total;
  }
};
}  // namespace sudare
