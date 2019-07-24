#pragma once

#include <cstddef>  // size_t

class SPI {
  int m_fd;
  int m_clock;

 public:
  explicit SPI(int clock);
  virtual ~SPI();
  size_t write(const char* data, size_t size, int cs) const;
};
