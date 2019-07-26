#include <zmq.h>
#include <array>
#include <iostream>
#include <sstream>
#include "converter.hpp"
#include "error.hpp"
#include "publisher.hpp"
#include "udp_server.h"
#include "zmq_utils.h"

#define CUBE_WIDTH 16
#define CUBE_HEIGHT 32
#define CUBE_DEPTH 8
#define CUBE_PKT_SIZE CUBE_WIDTH *CUBE_HEIGHT *CUBE_DEPTH * 2
#define SUDARE_ANGLES 360 / 6
#define SUDARE_RADIUS 15
#define SUDARE_HEIGHT 100

int main(int argc, const char *argv[]) {
  try {
    if (argc <= 3)
      throw std::runtime_error("input [ZMQ PORT] [UDP PORT] [spi CLOCK(MHz)]");
    sudare::zmq_initializer zmq_init;
    sudare::zmq_server zmq(zmq_init.context(), atoi(argv[1]));
    sudare::udp_server udp(atoi(argv[2]));
    sudare::spi_publisher publisher(atoi(argv[3]) * 1000 * 1000);
    zmq_pollitem_t items[] = {{zmq.socket(), 0, ZMQ_POLLIN, 0},
                              {0, udp.fd(), ZMQ_POLLIN, 0}};
    std::array<char, CUBE_PKT_SIZE> cube;
    sudare::rectangular rect(CUBE_WIDTH, CUBE_HEIGHT, CUBE_DEPTH);
    sudare::polar polar(SUDARE_ANGLES, SUDARE_RADIUS, SUDARE_HEIGHT);
    sudare::bilinear_converter convert(rect, polar);
    for (int n = 0;;) {
      if (zmq_poll(items, 2, -1) < 0) sudare::error("zmq_poll");
      if (items[0].revents & ZMQ_POLLIN) {
        int size = zmq.recv(polar.data(), polar.size());
        printf("%08d ZMQ Packet size : %d\n", ++n, size);
        if (size != polar.size()) continue;
        publisher(polar.data(), polar.size());
      }
      if (items[1].revents & ZMQ_POLLIN) {
        int size = 0;
        // UDPのバッファを空にして、最新のデータだけ受け取る
        for (;;) {
          int res = udp.recv(cube.data(), cube.size());
          if (res < 0) break;
          size = res;
        }
        printf("%08d UDP Packet size : %d\n", ++n, size);
        if (size != cube.size()) continue;
        rect.set_from_3d_led_pkt(cube.data());
        convert();
        publisher(polar.data(), polar.size());
      }
    }
  } catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;  // unreachable
}
