import sys, random, time, pyaudio, threading
from copy import deepcopy
import numpy as np
from os.path import abspath
from os import listdir
sys.path.append(abspath("build/lib.macosx-10.9-universal2-cpython-312"))
sys.path.append(abspath("build\\lib.win-amd64-cpython-312"))
sys.path.append(abspath("build/lib.linux-x86_64-cpython-311"))
import pyNES,pygame
#import tensorflow as tf

pygame.init()
window_dim = [256,240]
window = pygame.display.set_mode(window_dim,pygame.RESIZABLE)
nes_surf = pygame.Surface((240,256))
p = pyaudio.PyAudio()

stream = p.open(format = p.get_format_from_width(2),channels=1,rate=44100,output=True,frames_per_buffer = 4096)

nesObj = pyNES.NES(abspath("../../res/working_roms/Super Mario Bros. (JU) [!].nes"))
print("SAVEDIR:",nesObj.getSaveDir())
success = nesObj.setSaveDir(abspath("./mario_states"))
print("NEWSAVEDIR:",nesObj.getSaveDir(),success)
controller_port1 = pyNES.Controller()
nesObj.setController(controller_port1,0)
cpu_mem = nesObj.cpuMem()
running = True
matches = np.arange(0,0x10000)
last_mem = deepcopy(cpu_mem)
def mem_s(ind):
    return str(cpu_mem[ind])

nesObj.start()

def draw_image():
    frame = deepcopy(nesObj.getImg())
    grey = (np.sum(frame,2)/3).repeat(3).reshape(240,256,3).astype(np.uint8)
    frame = grey
    pygame.pixelcopy.array_to_surface(nes_surf,frame)
    scaled_ind = int(240/256<window_dim[1]/window_dim[0])
    scale_fac = [window_dim[0]/256,window_dim[1]/240][1-scaled_ind]
    nes_window_pos = [0,0]
    nes_window_pos[scaled_ind] = (window_dim[scaled_ind]-[256*scale_fac,240*scale_fac][scaled_ind])/2
    window.blit(pygame.transform.scale_by(pygame.transform.flip(pygame.transform.rotate(nes_surf,-90),True,False),scale_fac),nes_window_pos)

while running:
    c = nesObj.getAudio()
    stream.write(c)
    win = cpu_mem[0x5F3]
    fitnessFont = pygame.font.SysFont("arial",int(window_dim[1]/12))
    state = pygame.key.get_pressed()
    #random.choices([0,1],[1-1/(60*jump_seconds),1/(60*jump_seconds)][::(-1)**(last_a)])[0]
    keys = [state[pygame.K_SPACE],
            state[pygame.K_LSHIFT],
            state[pygame.K_TAB],
            state[pygame.K_RETURN],
            state[pygame.K_UP],
            state[pygame.K_DOWN],
            state[pygame.K_LEFT],
            state[pygame.K_RIGHT]]
    level_pos = cpu_mem[0x6D]*256+cpu_mem[0x86]
    powerup = cpu_mem[0x756]
    time = cpu_mem[0x7F8]*100+cpu_mem[0x7F9]*10+cpu_mem[0x7FA]
    pygame.display.set_caption("Time: "+str(time))
    nesObj.setPaused(state[pygame.K_p])
    controller_port1.updateInputs(keys)
    draw_image()
    pygame.display.update()
    window.fill((0,0,0))
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            nesObj.stop()
            running = False
        elif event.type == pygame.VIDEORESIZE:
            window_dim = [event.w,event.h]
            window = pygame.display.set_mode(window_dim,pygame.RESIZABLE)
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_r:
                pass
            elif event.key == pygame.K_s:
                nesObj.saveState(saves)
                print("Saved state",saves)
                saves+=1
            elif event.key == pygame.K_d:
                tmp_mem = deepcopy(cpu_mem)
                nesObj.setPaused(True)
                type = int(input(
"""Type of search: 
0 - reset memory table
1 - exact value
2 - changed (since last search)
3 - unchanged (since last search)
4 - increased (since last search)
5 - decreased (since last search)
"""))
                nesObj.setPaused(False)
                if type == 0:
                    matches = np.arange(0,0x10000)
                    print("Values reset!")
                elif (type == 1):
                    s = input("Val to search for:")
                    val = int(s)
                    matches = np.intersect1d(matches,np.arange(0,0x10000)[tmp_mem == val])
                    print(matches,matches.size)
                elif (type == 2):
                    matches = matches[last_mem[matches]!=tmp_mem[matches]]
                    print(matches,matches.size)
                elif (type == 3):
                    matches = matches[last_mem[matches]==tmp_mem[matches]]
                    print(matches,matches.size)
                elif (type == 4):
                    matches = matches[last_mem[matches]<tmp_mem[matches]]
                    print(matches,matches.size)
                elif (type == 5):
                    matches = matches[last_mem[matches]>tmp_mem[matches]]
                    print(matches,matches.size)
                last_mem = tmp_mem.copy()
stream.close()
p.terminate()