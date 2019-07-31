import cv2
import sys
import ctypes

img = cv2.imread('pikachu.png')
if img is None:
  print('failed to read image file')
  sys.exit(1)

lib = ctypes.cdll.LoadLibrary('libsudare.so')

if lib.sudare_init_sdk('192.168.11.20:5510') is not 0:
  sys.exit(1)

size = (45, 100)
img = cv2.resize(img, size)
cv2.imshow('image', img)
cv2.waitKey(1)

img = cv2.flip(img, 0)
img = cv2.flip(img, 1)

while True:
  lib.sudare_clear()
  for a in range(0, size[0]):
    for y in range(0, size[1]):
      for r in range(14, 15):
        c = img[y][a]
        rgb = (c[2] << 16) | (c[1] << 8) | c[0]
        lib.sudare_set_led_polar(a - 15, r, y, rgb)
  if lib.sudare_send() is not 0:
    sys.exit(1)
  lib.sudare_sleep(100)
  
