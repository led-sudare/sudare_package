#include <zmq.h>
#include <array>
#include <iostream>
#include <sstream>
#include "converter.h"
#include "error.hpp"
#include "publisher.h"
#include "zmq_utils.h"

int main(int argc, const char *argv[]) {
  try {
    if (argc <= 2)
      throw std::runtime_error(
          "input [ZMQ FROM ex:192.168.11.20:5511] [spi CLOCK(MHz)]");
    sudare::zmq_initializer zmq_init;
    sudare::zmq_server zmq(zmq_init.context(), argv[1]);
    sudare::spi_mini_publisher publisher(atoi(argv[2]) * 1000 * 1000);
    zmq_pollitem_t items[] = {{zmq.socket(), 0, ZMQ_POLLIN, 0}};
    sudare::polar polar(SUDARE_ANGLES, SUDARE_RADIUS, SUDARE_HEIGHT);
    for (int n = 0;;) {
      if (zmq_poll(items, 1, -1) < 0) sudare::error("zmq_poll");
      if (items[0].revents & ZMQ_POLLIN) {
        int size = 0;
        // 最新のデータだけ欲しいので、バッファが空になるまで受信を続ける
        for (;;) {
          int res = zmq.recv(polar.data(), polar.size(), ZMQ_NOBLOCK);
          ++n;
          if (res < 0) break;
          size = res;
        }
        printf("%08d ZMQ Packet size : %d\n", n, size);
        if (size - polar.size()) continue;
        publisher(polar.data(), polar.size());
      }
    }
  } catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;  // unreachable
}
