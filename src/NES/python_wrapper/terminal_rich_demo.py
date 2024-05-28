import sys, pyaudio, threading

import numpy as np
from os.path import abspath
sys.path.append(abspath("build/lib.macosx-10.9-universal2-cpython-312"))
sys.path.append(abspath("build\\lib.win-amd64-cpython-38"))
import pyNES
from rich.console import Console
from rich.style import Style
from rich.color import Color
from rich.segment import Segment, Segments
from rich.live import Live
from time import sleep

p = pyaudio.PyAudio()

stream = p.open(format = p.get_format_from_width(2),channels=1,rate=44100,output=True)

console = Console()
if len(sys.argv)>=2:
    nesObj = pyNES.NES(sys.argv[1])
else:
    quit("Wrong usage!")

def row(c1,c2):
    print(np.shape(c1),np.shape(c2))
    return 
color_lookup = nesObj.colorLookup()
segments = {}
for i in color_lookup:
    for j in color_lookup:
        segments[i.tobytes()+j.tobytes()]=Segment("\u2584",Style(color=Color.from_rgb(*i),bgcolor=Color.from_rgb(*j)))

def np_index(arr,val):
    return np.min(np.nonzero(arr == val)[0])

def terminal_draw(img: np.array):
    cols = np.shape(img)[1]
    segments_l = []
    for r in zip(img[::2,:,:],img[1::2,:,:]):
        c1, c2 = r
        for c in range(cols):
            try:  
                segments_l.append(segments[c2[c].tobytes()+c1[c].tobytes()])
            except KeyError:
                segments_l.append(segments_l[-1])
        segments_l.append(Segment.line())
    return Segments(segments_l,False)

nesObj.start()

def audio():
    global nesObj, stream
    while True:
        c = nesObj.getAudio()
        stream.write(c)

tAudio = threading.Thread(target=audio,daemon =True)
tAudio.start()
        
with Live(terminal_draw(nesObj.getImg()[::4,::3,:]),refresh_per_second = 60) as live:
    while True:
        v = terminal_draw(nesObj.getImg()[::4,::3,:])
        live.update(v)
        