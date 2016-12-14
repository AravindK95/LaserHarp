from __future__ import print_function
import sys
import time
import serial
import struct
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

def collect_data(s, delay):
    data = []
    s.read(s.in_waiting)
    start = time.time()
    while time.time() - start < delay:
        p0 = struct.unpack('B', s.read())[0]
        if p0 >> 7 != 0:
            continue
        p1 = struct.unpack('B', s.read())[0]

        pos = (p0 >> 4) & (0x7)
        val = ((p0 & 0xF) << 7) | (p1 & 0x7F)
        data.append((pos, val))
    return data

octaves = int(raw_input("How many octaves?"))
delay = int(raw_input("How many seconds of calibration at each stage?"))

s = serial.Serial(sys.argv[1], 115200)

data = []

raw_input("Press enter to calibrate ceiling...")
print("Calibrating ceiling...")
data.append(collect_data(s, delay))
print("Done")
print()
for octave in range(octaves):
    raw_input("Press enter to calibrate octave %d..." % octave)
    data.append(collect_data(s, delay))
    print("Done")
    print()

with open(sys.argv[2], "w") as f:
    f.write(str(data))

# print(ceiling_data)

# print(octaves_data)

# with open("octave_data.txt") as f:
#     data = f.read().replace('\n', '')

# data = eval(data)

for lvl in range(3):
    for bar in range(8):
        bar_data = [d[1] for d in data[lvl] if d[0] == bar]
        ax = plt.subplot(3, 8, lvl*8 + bar + 1)
        plt.hist(bar_data)
        ax.xaxis.set_major_locator(ticker.MultipleLocator(20))
        plt.title("level: "+str(lvl)+", bar: "+str(bar))

plt.show()