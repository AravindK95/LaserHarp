"""PyAudio Example: Play a WAVE file."""

import pyaudio
import wave
import sys
import threading
import time
import serial

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

def play_wav(p, filename):
    wf = wave.open(filename, 'rb')

    stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
                    channels=wf.getnchannels(),
                    rate=wf.getframerate(),
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
while True:
    byte = int(s.read()[0])
    for i in range(8):
        if (byte & (1 << i)) != 0 and (last_byte & (1 << i)) == 0:
            threading.Thread(target=play_wav, args=(p, samples[i])).start()
    last_byte = byte
    print(bin(byte))

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


