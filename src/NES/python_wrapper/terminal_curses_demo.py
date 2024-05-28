import sys, pyaudio
import numpy as np
from os.path import abspath
sys.path.append(abspath("build/lib.macosx-10.9-universal2-cpython-312"))
sys.path.append(abspath("build\\lib.win-amd64-cpython-38"))
import pyNES
from curses import wrapper
import curses

def main(s):
    s.clear()
    print("COLOR COUNT:",curses.COLORS,curses.COLOR_PAIRS)
    height, width = s.getmaxyx()
    width-=1
    p = pyaudio.PyAudio()

    stream = p.open(format = p.get_format_from_width(2),channels=1,rate=44100,output=True)

    if len(sys.argv)>=2:
        nesObj = pyNES.NES(sys.argv[1])
    else:
        quit("Wrong usage!")
    color_lookup = nesObj.colorLookup()
    #for i in range(64):
        #curses.init_color(i,*color_lookup[i])
        #curses.init_pair(i+1,i,i)
    curses.init_pair(111,curses.COLOR_RED,curses.COLOR_RED)
    nesObj.start()
    while True:
        terminal_draw(nesObj.getImg(),color_lookup,s)
        c = nesObj.getAudio()
        stream.write(c)

def terminal_draw(img: np.array,color_lookup,stdscr):
    rows,cols,color = np.shape(img)
    segments_l = []
    scalex,scaley = [12,12]
    for r in range(0,rows,scaley):
        for c in range(0,cols,scalex):  
            print(int(c/scalex),int(r/scaley))
            stdscr.addstr(int(c/scalex),int(r/scaley)," ",curses.color_pair(1))
            #stdscr.addstr("\u2584",style=Style(color=Color.from_rgb(*c2[c]),bgcolor=Color.from_rgb(*c1[c])))



wrapper(main)
        