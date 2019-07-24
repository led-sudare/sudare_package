#pragma once

#include <sstream>

namespace sudare {
inline void error(const char *msg, int err = errno) {
  std::stringstream ss;
  ss << "[" << msg << "] " << strerror(err);
  throw std::runtime_error(ss.str().c_str());
}
}  // namespace sudare
