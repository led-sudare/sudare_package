#pragma once

#include <cstddef>  // size_t

namespace sudare {
class spi {
  int m_fd;
  int m_clock;

 public:
  explicit spi(int clock);
  virtual ~spi();
  size_t write(const char* data, size_t size, int cs) const;
};
}  // namespace sudare
