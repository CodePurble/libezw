import pywt
import numpy as np
image = open("eggs-8x8.png.bin", "r")
a = np.fromfile(image, dtype=np.double)

print(a)
