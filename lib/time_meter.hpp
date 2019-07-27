#pragma once

#include <iostream>
#include <thread>  // std::this_thread::sleep_for

namespace sudare {
class time_meter {
  std::chrono::time_point<std::chrono::system_clock> m_start;
  std::string m_symbol;

 public:
  explicit time_meter(const char* symbol)
      : m_start(std::chrono::system_clock::now()), m_symbol(symbol) {}
  virtual ~time_meter() {
    auto end = std::chrono::system_clock::now();
    int diff = static_cast<int>(
        std::chrono::duration_cast<std::chrono::milliseconds>(end - m_start)
            .count());
    std::cout << m_symbol << " : " << diff << "ms" << std::endl;
  }
};
}  // namespace sudare
