#pragma once

#include <string.h>  // strerror
#include <sstream>

namespace sudare {
inline void panic(const char *msg, int err = errno) {
  std::stringstream ss;
  ss << "[" << msg << "] " << strerror(err);
  throw std::runtime_error(ss.str().c_str());
}
}  // namespace sudare
