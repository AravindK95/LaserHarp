import numpy as np
import matplotlib.pyplot as plt
import sys

with open(sys.argv[1]) as f:
	lines = f.readlines()

plt.figure()

for row in lines:
	r = row.split()
	r = map(float, r)
	plt.plot(r[1:])

plt.show()