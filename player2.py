"""PyAudio Example: Play a WAVE file."""

import pyaudio
import wave
import sys
import threading
import time
import serial

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
#"samples/68439__pinkyfinger__piano-bb.wav",
"samples/68440__pinkyfinger__piano-c.wav",
#"samples/68441__pinkyfinger__piano-c.wav",
"samples/68442__pinkyfinger__piano-d.wav",
"samples/68443__pinkyfinger__piano-e.wav",
#"samples/68444__pinkyfinger__piano-eb.wav",
"samples/68445__pinkyfinger__piano-f.wav",
#"samples/68446__pinkyfinger__piano-f.wav",
"samples/68448__pinkyfinger__piano-g.wav"]

def play_wav(p, filename, octave=1):
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

# if len(sys.argv) < 2:
#     print("Plays a wave file.\n\nUsage: %s filename.wav" % sys.argv[0])
#     sys.exit(-1)

# wf = wave.open(sys.argv[1], 'rb')

p = pyaudio.PyAudio()
# threads = [threading.Thread(target=play_wav, args=(p, sample)) for sample in samples*3]
# for thread in threads:
#     thread.start()
#     time.sleep(.15)
# for thread in threads:
#     thread.join()

s = serial.Serial(sys.argv[1], 115200)
s.read(s.in_waiting)
last_byte=0

vals = [0]*8

while True:
    pos = int(s.read()[0])
    if (pos > 7):
        continue
    val = int(s.read()[0]) - 10
    if (val != vals[pos]):
        vals[pos] = val
        if val:
            print(pos, cmajor[pos], cmajor[pos]-13+12*val)
            threading.Thread(target=play_wav, args=(p, "samples2/" + samples2[cmajor[pos]-13+12*1], 1)).start()
    # for i in range(8):
    #     if (byte & (1 << i)) != 0 and (last_byte & (1 << i)) == 0: samples2[cmajor[pos]+12*val-13]
    #         threading.Thread(target=play_wav, args=(p, samples[i])).start()
    # last_byte = byte
    print(pos, val)

p.terminate()

# stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
#                 channels=wf.getnchannels(),
#                 rate=wf.getframerate(),
#                 output=True)

# data = wf.readframes(CHUNK)

# while data != '':
#     stream.write(data)
#     data = wf.readframes(CHUNK)

# stream.stop_stream()
# stream.close()


