#include "udp_server.h"
#include <sys/ioctl.h>  // ioctl
#include <unistd.h>     // close
#include <cstring>      // strerror
#include <iostream>
#include "panic.hpp"

namespace sudare {
void udp_server::close() {
  if (0 < m_fd && ::close(m_fd) < 0) panic("close");
  m_fd = 0;
}

udp_server::udp_server(int port, int nonblock) : m_fd(0), m_addr({0}) {
  std::cout << "UDP PORT : " << port << std::endl;
  m_fd = ::socket(AF_INET, SOCK_DGRAM, 0);
  if (m_fd < 0) panic("socket");
  m_addr.sin_family = AF_INET;
  m_addr.sin_port = htons(port);
  m_addr.sin_addr.s_addr = INADDR_ANY;
  if (::bind(m_fd, reinterpret_cast<sockaddr *>(&m_addr), sizeof(m_addr)) < 0) {
    int err = errno;
    close();
    panic("bind", err);
  }
  if (!nonblock) return;
  int v = 1;  // non blocking
  if (::ioctl(m_fd, FIONBIO, &v) < 0) panic("ioctl(FIONBIO)");
}

udp_server::~udp_server() { close(); }

int udp_server::recv(char *buf, int size) const {
  int res = ::recv(m_fd, buf, size, 0);
  if (res < 0 && errno != EAGAIN) panic("recv");
  return res;
}
}  // namespace sudare
