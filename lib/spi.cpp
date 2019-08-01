#include "spi.h"
#include <unistd.h>  // close, usleep
#include <iostream>
#include "panic.hpp"
#ifdef ENABLE_SPI
#include <fcntl.h>  // open
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#endif  // ENABLE_SPI

namespace sudare {
spi::spi(int clock) : m_clock(clock) {
  std::cout << "spi CLOCK : " << clock << "Hz" << std::endl;
#ifdef ENABLE_SPI
  const char *dev = "/dev/spidev0.0";
  int res = open(dev, O_RDWR);
  if (res < 0) panic("open(/dev/spidev0.0)");
  m_fd = res;
#endif  // ENABLE_SPI
}

spi::~spi() {
#ifdef ENABLE_SPI
  if (close(m_fd) < 0) panic("close");
#endif  // ENABLE_SPI
}

size_t spi::write(const char *data, size_t size, int cs) const {
#ifdef ENABLE_SPI
  spi_ioc_transfer msg[1] = {{0}};
  msg[0].tx_buf = (unsigned long)data;
  msg[0].len = size;
  msg[0].speed_hz = m_clock;
  msg[0].bits_per_word = 8;
  msg[0].cs_change = cs;
  int res = ioctl(m_fd, SPI_IOC_MESSAGE(1), &msg);
  if (res < 0) panic("ioctl(SPI_IOC_MESSAGE)");
#else
  double sec = size * 8.0 / m_clock;
  int ns = static_cast<int>(sec * 1000 * 1000);
  usleep(ns);  // SPIの通信時間だけ待機（シミュレーション）
  int res = size;
#endif  // ENABLE_SPI
  return static_cast<size_t>(res);
}
}  // namespace sudare
