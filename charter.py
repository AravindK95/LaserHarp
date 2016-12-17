import numpy as np
import matplotlib.pyplot as plt
import sys

colors = [
'b',
'g',
'r',
'c',
'm',
'y',
'k',
'w'
]


with open(sys.argv[1]) as f:
	lines = f.readlines()

plt.figure()

for row in lines:
	r = row.split()
	r = map(float, r)
	# if int(r[0]) != 0:
	# 	continue
	plt.plot(r[1:], c=colors[int(r[0])])

plt.show()