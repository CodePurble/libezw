#! /usr/bin/env python3

import numpy
import pywt
from pprint import pprint
arr = []
i = 0
with open("imgbin/eggs-8x8.png.bin", "rb") as f:
    byte = f.read(1)
    while byte:
        arr.append(int.from_bytes(byte, "big"))
        byte = f.read(1)

# print(arr)

arr1 = numpy.reshape(arr, (8,8))
pprint(arr1)

coeffs = pywt.wavedec2(arr1,'haar')
pprint(coeffs)

f.close()
