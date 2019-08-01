#include "sudare.h"
#include <zmq.h>
#include <iostream>
#include <memory>  // std::shared_ptr
#include <thread>  // std::this_thread::sleep_for
#include "controller.h"
#include "converter.h"
#include "publisher.h"

namespace {
sudare::zmq_initializer s_zmq_init;
sudare::bilinear_converter s_convert;
std::shared_ptr<sudare::publisher> s_sender;
std::shared_ptr<sudare::controller> s_ctrl;
}  // namespace

int sudare_init_sdk(const char* dst) {
  try {
    void* context = s_zmq_init.context();
    s_sender = std::make_shared<sudare::zmq_publisher>(context, dst);
    s_ctrl = std::make_shared<sudare::controller>(s_convert, *s_sender);
    return 0;
  } catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

int sudare_set_led_rect(int x, int y, int z, int rgb) {
  try {
    s_ctrl->set_led_rect(x, y, z, rgb);
    return 0;
  } catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

int sudare_set_led_polar(int a, int r, int h, int rgb) {
  try {
    s_ctrl->set_led_polar(a, r, h, rgb);
    return 0;
  } catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

int sudare_clear(void) {
  try {
    s_ctrl->clear();
    return 0;
  } catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

int sudare_send(void) {
  try {
    s_ctrl->send();
    return 0;
  } catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

int sudare_sleep(int ms) {
  try {
    s_ctrl->sleep(ms);
    return 0;
  } catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
