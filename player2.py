""" Code adapted from PyAudio sample program at https://people.csail.mit.edu/hubert/pyaudio/"""
import pyaudio
import wave
import sys
import threading
import time
import serial
import struct
from subprocess import Popen, PIPE

from samples import samples2

cmajor = {
         0: 40,
         1: 42, 
         2: 44, 
         3: 45, 
         4: 47, 
         5: 49, 
         6: 51, 
         7: 52}


CHUNK = 1024

samples = [
"samples/68447__pinkyfinger__piano-g.wav",
"samples/68437__pinkyfinger__piano-a.wav",
"samples/68438__pinkyfinger__piano-b.wav",
"samples/68440__pinkyfinger__piano-c.wav",
"samples/68442__pinkyfinger__piano-d.wav",
"samples/68443__pinkyfinger__piano-e.wav",
"samples/68445__pinkyfinger__piano-f.wav",
"samples/68448__pinkyfinger__piano-g.wav"]

flute_samples = [
"flute/C.mp3",
"flute/D.mp3",
"flute/E.mp3",
"flute/F.mp3",
"flute/G.mp3",
"flute/A.mp3",
"flute/B.mp3",
"flute/high_C.mp3"]

xylophone_samples = [
"xylophone/232002__danmitch3ll__xylophone-c.wav",
"xylophone/232008__danmitch3ll__xylophone-d1.wav",
"xylophone/232007__danmitch3ll__xylophone-e1.wav",
"xylophone/232006__danmitch3ll__xylophone-f.wav",
"xylophone/232005__danmitch3ll__xylophone-g.wav",
"xylophone/232004__danmitch3ll__xylophone-a.wav",
"xylophone/232003__danmitch3ll__xylophone-b.wav",
"xylophone/232001__danmitch3ll__xylophone-c2.wav"]

def zscore(x, mean, var):
    return (x - mean) / (var**0.5)

def octave(pos, val):
    #print(thresholds)
    scores = [abs(zscore(val, t[0], t[1])) for t in thresholds[pos]]
    if min([0, 2, 1], key=lambda x: scores[x]):
        print(scores)
    return min([0, 2, 1], key=lambda x: scores[x])

def play_wav(p, filename, octave=1):
    print("opening", filename)
    wf = wave.open(filename, 'rb')

    stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
                    channels=wf.getnchannels(),
                    rate=wf.getframerate() * octave,
                    output=True)

    data = wf.readframes(CHUNK)

    while data != '':
        stream.write(data)
        data = wf.readframes(CHUNK)

    stream.stop_stream()
    stream.close()
def mean(items):
    return sum(items) / len(items)
def variance(items):
    m = mean(items)
    return sum([(item - m) ** 2 for item in items]) / len(items)
def read_calibration(filename):
    data = eval(open(filename).read())
    thresholds = [[0 for _ in range(2)] for __ in range(8)]

    beams = [[[], [], []], [[], [], []], [[], [], []], [[], [], []], [[], [], []], [[], [], []], [[], [], []], [[], [], []]]
    for index, layer in enumerate(data[0:3]):
        for pos, val in layer:
            beams[pos][index].append(val)

    stats = []
    print(beams)
    for beam in beams:
        beam_stats = []
        for layer in beam:
            beam_stats += [[mean(layer), variance(layer)]]
        stats += [beam_stats]
    return stats


thresholds = read_calibration(sys.argv[2])
print(thresholds)
p = pyaudio.PyAudio()

s = serial.Serial(sys.argv[1], 115200)
s.read(s.in_waiting)
last_byte=0

vals = [0]*8
count = [0]*8

while True:
    p0 = struct.unpack('B', s.read())[0]
    if p0 >> 7 != 0:
        continue
    p1 = struct.unpack('B', s.read())[0]

    pos = (p0 >> 4) & (0x7)
    val = ((p0 & 0xF) << 7) | (p1 & 0x7F)
    val = octave(pos, val)
    # only play sounds when status changes
    if (val != vals[pos]):
        vals[pos] = val
        if val == 2:
            threading.Thread(target=play_wav, args=(p, "samples2/" + samples2[cmajor[pos]-13+12*1], 1)).start()
        elif val == 1:
            threading.Thread(target=play_wav, args=(p, xylophone_samples[pos], 1)).start()
    

p.terminate()
