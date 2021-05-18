#!/usr/bin/env python3
from PIL import Image

img = Image.open('Lichtenstein_img_processing_test_bw.png').convert('L')
WIDTH, HEIGHT = img.size

data = list(img.getdata())
data_bytes = bytes(img.getdata())
#  data = [data[offset:offset+WIDTH] for offset in range(0, WIDTH*HEIGHT, WIDTH)]

with open("raw_lichtenstein_img_processing_test_bw.bin", "wb") as f:
    f.write(data_bytes)
#  for row in data:
#      for value in row:
#          #  print(' '.join('{:3}'.format(value)), end='')
#          print(str(value) + ',', end='')
#      print()

#  chars = '@%#*+=-:. '  # Change as desired.
#  scale = (len(chars)-1)/255.
#  for row in data:
#      for value in row:
#          print(chars[int(value*scale)], end='')
#      print()
