#pragma once

#include <netinet/in.h>  // sockaddr_in, socket, bind, recv, etc...

namespace sudare {
class udp_server {
  int m_fd;
  sockaddr_in m_addr;
  void close();

 public:
  explicit udp_server(int port);
  virtual ~udp_server();
  int recv(char *buf, int size) const;
  int fd() const { return m_fd; }
};
}  // namespace sudare
