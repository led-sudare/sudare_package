#include "zmq_utils.h"

#include <zmq.h>
#include <iostream>
#include <sstream>

namespace sudare {
namespace {
void error(const char* msg, int err = zmq_errno()) {
  std::stringstream ss;
  ss << "[" << msg << "] " << zmq_strerror(err);
  throw std::runtime_error(ss.str().c_str());
}
}  // namespace

zmq_initializer::zmq_initializer() {
  m_context = zmq_ctx_new();
  if (m_context == 0) error("zmq_ctx_new");
}
zmq_initializer::~zmq_initializer() {
  if (zmq_ctx_shutdown(m_context) < 0) error("zmq_ctx_shutdown");
}

zmq_server::zmq_server(void* context, const char* from) {
  std::string const tcp = "tcp://" + std::string(from);
  connect(context, tcp.c_str());
}

zmq_server::zmq_server(void* context, int port) {
  std::string const tcp = "tcp://0.0.0.0:" + std::to_string(port);
  connect(context, tcp.c_str());
}

zmq_server::~zmq_server() {
  if (zmq_close(m_socket) < 0) error("zmq_close");
}

void zmq_server::connect(void* context, const char* tcp) {
  m_socket = zmq_socket(context, ZMQ_SUB);
  if (m_socket == 0) error("zmq_socket");
  std::cout << "ZMQ SERVER : " << tcp << std::endl;
  if (zmq_connect(m_socket, tcp) < 0) error("zmq_connect");
  if (zmq_setsockopt(m_socket, ZMQ_SUBSCRIBE, "", 0) < 0)
    error("zmq_setsockopt");
}

int zmq_server::recv(char* buf, int size, int flags) const {
  int res = zmq_recv(m_socket, buf, size, flags);
  if (res < 0 && zmq_errno() != EAGAIN) error("zmq_recv");
  std::cout << "zmq received : " << res << "bytes" << std::endl;
  return res;
}

zmq_client::zmq_client(void* context, const char* dst) {
  m_socket = zmq_socket(context, ZMQ_PUB);
  if (m_socket == 0) error("zmq_socket");
  std::string const tcp = "tcp://" + std::string(dst);
  std::cout << "ZMQ CLIENT : " << tcp << std::endl;
  if (dst[0] == '*') {
    if (zmq_bind(m_socket, tcp.c_str()) < 0) error("zmq_bind");
  } else {
    if (zmq_connect(m_socket, tcp.c_str()) < 0) error("zmq_connect");
  }
}

zmq_client::~zmq_client() {
  if (zmq_close(m_socket) < 0) error("zmq_close");
}

int zmq_client::send(const char* buf, int size) const {
  int res = zmq_send(m_socket, buf, size, 0);
  if (res < 0) error("zmq_send");
  std::cout << "zmq sent : " << res << "bytes" << std::endl;
  return res;
}
}  // namespace sudare
