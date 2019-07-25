#pragma once

#include "zmq_utils.h"

namespace sudare {
class publisher {
 public:
  virtual ~publisher() {}
  virtual void publish(const char* data, size_t size) = 0;
};
class zmq_sender : public publisher {
  sudare::zmq_initializer m_init;
  sudare::zmq_client m_client;

 public:
  zmq_sender(const char* dst) : m_init(), m_client(m_init.context(), dst) {}
  ~zmq_sender() {}
  void publish(const char* data, size_t size) { m_client.send(data, size); }
};
}  // namespace sudare
