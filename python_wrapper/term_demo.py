import omnicom
from math import sqrt
from time import time, sleep
from colorsys import hsv_to_rgb
from os import system
from requests import get
from io import BytesIO
import numpy as np
from pynput import keyboard
from functools import lru_cache
from tty import setraw, setcbreak
from sys import stdout,stdin
from os import get_terminal_size
from os.path import exists
import socket
import traceback
import argparse

parser = argparse.ArgumentParser(prog="Terminal NES",usage="python term_demo.py [--hud] [--debug]")
parser.add_argument("--debug",action="store_true")
parser.add_argument("--hud",action="store_true")

args = parser.parse_args()
    
termcols,termrows = get_terminal_size()
setraw(stdin.fileno())

pressed = []
def key_pressed(key):
    global pressed
    if not (key in pressed):
        pressed.append(key)

def key_released(key):
    global pressed
    if key in pressed:
        pressed.remove(key)

listener = keyboard.Listener(on_press=key_pressed,on_release=key_released)
listener.start()

buttons = [False]*8
maps = [
    keyboard.Key.space,
    keyboard.Key.shift,
    keyboard.Key.tab,
    keyboard.Key.enter,
    keyboard.Key.up,
    keyboard.Key.down,
    keyboard.Key.left,
    keyboard.Key.right
]

url ="https://www.nesfiles.com/NES/Super_Mario_Bros_3/Super_mario_brothers3.nes"

rom = BytesIO(get(url).content)
rom.name = url.split("/")[-1]
nes_obj = omnicom.NES(rom)
    
sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
if args.debug:
    sock.connect(("localhost",1234))

def debug_console(*msgs):
    global sock
    if args.debug:
        sock.send((" ".join([repr(msg) if type(msg) != str else msg for msg in msgs])+"\n").encode("utf-8"))
#nes_obj = omnicom.NES("/Users/andrewogundimu/code_projects/NES/res/roms/Super Mario Bros. (JU) [!].nes")
#nes_obj = omnicom.NES("/Users/andrewogundimu/code_projects/NES/res/roms/helloworld.nes")
controller = omnicom.Controller()
nes_obj.setController(controller,0)
nes_obj.start()
img = nes_obj.getImg()
cpu_mem = nes_obj.cpuMem()
start = time()
ansi_colors = []

ansi_colors.append([0,0,0])
ansi_colors.append([0x80,0,0])
ansi_colors.append([0,0x80,0])
ansi_colors.append([0x80,0x80,0])
ansi_colors.append([0,0,0x80])
ansi_colors.append([0x80,0,0x80])
ansi_colors.append([0,0x80,0x80])
ansi_colors.append([0xc0,0xc0,0xc0])
ansi_colors.append([0x80,0x80,0x80])
for i in range(6):
    ansi_colors.append([255 if x !=0 else 0 for x in ansi_colors[i+1]])
ansi_colors.append([0,0,0])
c_scale = [0,95,135,175,215,255]
for i in range(216):
    b = c_scale[(i%6)]
    g = c_scale[(i//6)%6]
    r = c_scale[(i//6//6)%6]
    ansi_colors.append([r,g,b])
for i in range(24):
    ansi_colors.append([8+i*10,8+i*10,8+i*10])

ansi_colors = np.array(ansi_colors)
  
@lru_cache(maxsize=None)
def rgb2ansi(r,g,b):
    global ansi_colors
    color = np.array([r,g,b])
    return np.argmin(np.sum((ansi_colors-color)**2,axis=1))

def hue2rgb(h): # assume max sat and lightness
    h*=6
    h%=6
    x = (1-abs(h%2-1))*255
    if 0<=h<=1:
        return (255,x,0)
    elif 1<=h<=2:
        return (x,255,0)
    elif 2<=h<=3:
        return (0,255,x)
    elif 3<=h<=4:
        return (0,x,255)
    elif 4<=h<=5:
        return (x,0,255)
    else:
        return (255,0,x)

on = 0
scale = 4
topleft = (termcols//2-128//scale,0)
elapsed = 0
lookup = [list(i) for i in nes_obj.colorLookup()]

def color_look(i):
    try:
        return ansi_lookup[lookup.index(i)]
    except ValueError:
        return rgb2ansi(*i)


def add_rows(r1,r2):
    return r1+r2

ansi_lookup = [rgb2ansi(*list(i)) for i in list(nes_obj.colorLookup())]
running = True
scaledim = np.zeros(img.shape)[::scale,::scale]
change_count = 0
try:
    system("clear")
    while running:
        #print(img[::4,::4].shape)
        last = elapsed
        elapsed = time()-start
        buttons = [i in pressed for i in maps]
        controller.updateInputs(buttons)

        scaledim = np.copy(img[::scale,::scale])
        for ind,r in enumerate(scaledim.reshape(-1,2,*scaledim.shape[1:])):
            up_row_colors = [f"\u001b[48;5;{color_look(tuple(i))}m" for i in r[0]]
            down_row_colors = [f"\u001b[38;5;{color_look(tuple(i))}m\u2584" for i in r[1]]
            final_print = f"\033[{ind+topleft[1]+1};{topleft[0]-1}H"
            final_print += "".join(list(map(add_rows,up_row_colors,down_row_colors)))
            print(final_print,end="")
        #generate score as a string
        score_str = ""
        score_mem = [112,113,114,115,116][::-1]
        score_len = -1
        for ind,i in enumerate(score_mem):
            digit = cpu_mem[i]
            if digit!=0 and score_len < 0:
                score_len = ind+2
            if score_len>=0:
                score_str+=str(digit)
        score_str+="0"
        
        debug_console("Score:",score_str)
        #debug_console("-----------------")
        if keyboard.KeyCode.from_char("q") in pressed:
            running = False
except Exception as e:
    debug_console(traceback.format_exc())
    running = False

#sock.shutdown(0)
sock.close()
setcbreak(stdin.fileno())
print(f'\u001b[39m\u001b[49m')
system("clear")
print("stop")
nes_obj.stop()