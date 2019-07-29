#pragma once

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
  zmq_publisher(void* context, const char* dst);
  ~zmq_publisher() {}
  int operator()(const char* data, size_t size);
};
class spi_publisher : public publisher {
  sudare::spi m_spi;

 public:
  explicit spi_publisher(int clock);
  int operator()(const char* data, size_t size);
};
}  // namespace sudare
