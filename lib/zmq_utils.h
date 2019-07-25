#pragma once

namespace sudare {
class zmq_initializer {
  void* m_context;

 public:
  zmq_initializer();
  virtual ~zmq_initializer();
  void* context() const { return m_context; };
};

class zmq_server {
  void* m_socket;

 public:
  zmq_server(void* context, int port);
  virtual ~zmq_server();
  int recv(char* buf, int size) const;
  void* socket() const { return m_socket; }
};

class zmq_client {
  void* m_socket;

 public:
  zmq_client(void* context, const char* dst);
  virtual ~zmq_client();
  int send(const char* buf, int size) const;
  void* socket() const { return m_socket; }
};
}  // namespace sudare
