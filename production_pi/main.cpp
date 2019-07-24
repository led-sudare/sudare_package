#include <zmq.h>
#include <array>
#include <iostream>
#include <sstream>
#include "converter.hpp"
#include "error.hpp"
#include "fpga_ctrl.h"
#include "udp_server.h"
#include "zmq_utils.h"

#define LED_WIDTH 16
#define LED_HEIGHT 32
#define LED_DEPTH 8
#define CUBE_PKT_SIZE LED_WIDTH *LED_HEIGHT *LED_DEPTH * 2

int main(int argc, const char *argv[]) {
  try {
    if (argc <= 3)
      throw std::runtime_error("input [ZMQ PORT] [UDP PORT] [spi CLOCK(MHz)]");
    sudare::zmq_initializer zmq_init;
    sudare::zmq_server zmq(zmq_init.context(), atoi(argv[1]));
    sudare::udp_server udp(atoi(argv[2]));
    sudare::fpga_ctrl fpga(atoi(argv[3]));
    zmq_pollitem_t items[] = {{zmq.socket(), 0, ZMQ_POLLIN, 0},
                              {0, udp.fd(), ZMQ_POLLIN, 0}};
    std::array<char, CUBE_PKT_SIZE> cube;
    sudare::rectangular rect(LED_WIDTH, LED_HEIGHT, LED_DEPTH);
    sudare::polar polar(SUDARE_ANGLES, SUDARE_WIDTH / 2, SUDARE_HEIGHT);
    sudare::converter conv(rect, polar);
    for (int n = 0;;) {
      if (zmq_poll(items, 2, -1) < 0) sudare::error("zmq_poll");
      if (items[0].revents & ZMQ_POLLIN) {
        int size =
            zmq.recv(reinterpret_cast<char *>(polar.data()), polar.size());
        printf("%08d ZMQ Packet size : %d\n", ++n, size);
        if (size == polar.size())
          fpga.write(reinterpret_cast<char *>(polar.data()));
      }
      if (items[1].revents & ZMQ_POLLIN) {
        int size = 0;
        for (;;) {
          int res = udp.recv(cube.data(), cube.size());
          if (res < 0) break;
          size = res;
        }
        printf("%08d UDP Packet size : %d\n", ++n, size);
        if (size == cube.size()) {
          rect.from3DLED(reinterpret_cast<uint8_t *>(cube.data()));
          conv();
          fpga.write(reinterpret_cast<char *>(polar.data()));
        }
      }
    }
  } catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;  // unreachable
}
