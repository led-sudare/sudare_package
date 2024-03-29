#pragma once

#include "polar.h"
#include "spi.h"
#include "zmq_utils.h"

namespace sudare {
class publisher {
 public:
  virtual ~publisher() {}
  virtual int operator()(polar const& po) const = 0;
};
class zmq_publisher : public publisher {
  sudare::zmq_client m_client;

 public:
  zmq_publisher(void* context, const char* dst);
  ~zmq_publisher() {}
  int operator()(polar const& po) const;
};
class spi_publisher : public publisher {
  sudare::spi m_spi;

 public:
  explicit spi_publisher(int clock);
  int operator()(polar const& po) const;
};
class spi_mini_publisher : public publisher {
  sudare::spi m_spi;

 public:
  explicit spi_mini_publisher(int clock);
  int operator()(polar const& po) const;
};
}  // namespace sudare
