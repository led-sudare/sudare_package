#include <array>
#include <cstdlib>
#include <iostream>
#include "converter.hpp"
#include "udp_server.h"
#include "zmq_utils.h"

#define LED_WIDTH 16
#define LED_HEIGHT 32
#define LED_DEPTH 8
#define CUBE_PKT_SIZE LED_WIDTH *LED_HEIGHT *LED_DEPTH * 2
#define SUDARE_WIDTH 30
#define SUDARE_HEIGHT 100
#define ANGLE_RESOLUTION 6
#define SUDARE_ANGLES 360 / ANGLE_RESOLUTION
#define SUDARE_PKT_SIZE SUDARE_WIDTH *SUDARE_HEIGHT *SUDARE_ANGLES

int main(int argc, const char *argv[]) {
  try {
    if (argc <= 2)
      throw std::invalid_argument(
          "You should input [MY PORT] [TARGET ex *:1234 or localhost:4567].");
    const int my_port = atoi(argv[1]);
    const char *target = argv[2];
    std::cout << "MY PORT : " << my_port << "\nTARGET PORT: " << target
              << std::endl;
    sudare::udp_server rx(my_port);
    sudare::zmq_initializer zmq_init;
    sudare::zmq_client tx(zmq_init.context(), target);
    sudare::rectangular rect(LED_WIDTH, LED_HEIGHT, LED_DEPTH);
    sudare::polar polar(SUDARE_ANGLES, SUDARE_WIDTH / 2, SUDARE_HEIGHT);
    sudare::converter convert(rect, polar);
    std::array<char, CUBE_PKT_SIZE> cube;
    for (;;) {
      int size = rx.recv(cube.data(), cube.size());
      if (size != cube.size()) {
        std::cerr << "invalid packet size " << size << " != " << cube.size()
                  << std::endl;
        continue;
      }
      rect.from3DLED(reinterpret_cast<uint8_t *>(cube.data()));
      convert();
      tx.send(reinterpret_cast<char *>(polar.data()), polar.size());
    }
  } catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;  // unreachable
}
