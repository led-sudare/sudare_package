import cv2
import sys
import ctypes

img = cv2.imread('doraemon.png')
if img is None:
  print('failed to read image file')
  sys.exit(1)

lib = ctypes.cdll.LoadLibrary('libsudare.so')

if lib.sudare_init_sdk('*:5511') != 0:
  sys.exit(1)

size = (30, 100)
img = cv2.resize(img, size)
cv2.imshow('image', img)
cv2.waitKey(1)

lib.sudare_clear()

for a in range(0, 1000):
  for x in range(0, size[0]):
    for y in range(0, size[1]):
      c = img[y][x]
      rgb = (c[2] << 16) | (c[1] << 8) | c[0]
      for r in range(13, 15):
        lib.sudare_set_led_polar(a + x, r, y, rgb)
  if lib.sudare_send() != 0:
    sys.exit(1)
  lib.sudare_sleep(100)