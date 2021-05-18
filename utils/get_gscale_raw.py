#!/usr/bin/env python3
from PIL import Image

img = Image.open('Lichtenstein_img_processing_test_bw.png').convert('L')
WIDTH, HEIGHT = img.size

data = list(img.getdata())
data = [data[offset:offset+WIDTH] for offset in range(0, WIDTH*HEIGHT, WIDTH)]
for row in data:
    for value in row:
        #  print(' '.join('{:3}'.format(value)), end='')
        print(str(value) + ',', end='')
    print()
