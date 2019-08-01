#include <array>
#include <cstdlib>
#include <iostream>
#include "converter.h"
#include "udp_server.h"
#include "zmq_utils.h"

#define CUBE_PKT_SIZE CUBE_WIDTH *CUBE_HEIGHT *CUBE_DEPTH * 2

int main(int argc, const char *argv[]) {
  try {
    if (argc <= 2)
      throw std::invalid_argument(
          "You should input [MY PORT] [TARGET ex *:1234 or localhost:4567].");
    const int my_port = atoi(argv[1]);
    const char *target = argv[2];
    std::cout << "MY PORT : " << my_port << "\nTARGET PORT: " << target
              << std::endl;
    sudare::udp_server rx(my_port, 0);
    sudare::zmq_initializer zmq_init;
    sudare::zmq_client tx(zmq_init.context(), target);
    sudare::rectangular rect(CUBE_WIDTH, CUBE_HEIGHT, CUBE_DEPTH);
    sudare::polar polar(SUDARE_ANGLES, SUDARE_RADIUS, SUDARE_HEIGHT);
    sudare::bilinear_converter convert;
    std::array<char, CUBE_PKT_SIZE> cube;
    for (;;) {
      int size = rx.recv(cube.data(), cube.size());
      if (size != cube.size()) {
        std::cerr << "invalid packet size " << size << " != " << cube.size()
                  << std::endl;
        continue;
      }
      rect.set_from_3d_led_pkt(cube.data());
      convert(rect, polar);
      tx.send(polar.data(), polar.size());
    }
  } catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;  // unreachable
}
