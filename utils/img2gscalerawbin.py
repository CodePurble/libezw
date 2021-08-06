#!/usr/bin/env python3
from PIL import Image
from sys import argv

ifname = argv[1]
ofname = ifname + ".bin"

img = Image.open(ifname).convert('L')
WIDTH, HEIGHT = img.size

data = list(img.getdata())
data_bytes = bytes(img.getdata())
#  data = [data[offset:offset+WIDTH] for offset in range(0, WIDTH*HEIGHT, WIDTH)]

with open(ofname, "wb") as f:
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
