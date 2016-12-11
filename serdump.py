import serial
import sys
s = serial.Serial(sys.argv[1], 115200)
s.read(s.in_waiting)

with open(sys.argv[2], "w") as f:
	while True:
	    f.write(s.read())