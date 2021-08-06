#!/usr/bin/env python3
import numpy as np
from PIL import Image
from sys import argv

ifname = argv[1]
ofname = ifname.split(".")[0] + ".png"
dim = int(argv[2])

bytes_in = np.array(0)
with open(ifname, "rb") as f:
    bytes_in = list(f.read())

arr = np.asarray(bytes_in, dtype=np.uint8)
arr = np.reshape(arr, (dim, dim))

img = Image.fromarray(arr)
img.save(ofname)
