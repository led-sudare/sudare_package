#include <sudare.h>
#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, const char* argv[]) {
  if (argc < 3) {
    std::cerr << "input [ZMQ destination] [video file]" << std::endl;
    return 1;
  }
  if (sudare_init_sdk(argv[1])) return 1;
  for (;;) {
    cv::VideoCapture cap(argv[2]);
    if (!cap.isOpened()) {
      std::cerr << "failed to open video file" << std::endl;
      return 1;
    }
    for (;;) {
      cv::Mat m;
      cap >> m;
      if (m.empty()) break;
      cv::resize(m, m, cv::Size(30, 100));
      cv::imshow("image", m);
      cv::waitKey(1);
      cv::flip(m, m, 0);
      cv::flip(m, m, 1);
      for (int a = 0; a < 30; ++a) {
        for (int r = 0; r < 15; ++r) {
          int x0 = 14 - r;
          int x1 = 15 + r;
          for (int y = 0; y < 100; ++y) {
            auto v0 = m.at<cv::Vec3b>(y, x0);
            int rgb0 = (v0[2] << 16) + (v0[1] << 8) + (v0[0] & 0xFF);
            sudare_set_led_polar(a, r, y, rgb0);
            auto v1 = m.at<cv::Vec3b>(y, x1);
            int rgb1 = (v1[2] << 16) + (v1[1] << 8) + (v1[0] & 0xFF);
            sudare_set_led_polar(a + 30, r, y, rgb1);
          }
        }
      }
      sudare_send();
    }
  }
  return 0;
}
