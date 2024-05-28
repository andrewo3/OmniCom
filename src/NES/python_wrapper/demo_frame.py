import sys, random, time, pyaudio, threading
from copy import deepcopy
import numpy as np
from os.path import abspath
sys.path.append(abspath("build/lib.macosx-10.9-universal2-cpython-312"))
sys.path.append(abspath("build\\lib.win-amd64-cpython-312"))
sys.path.append(abspath("build/lib.linux-x86_64-cpython-311"))
import pyNES,pygame

pygame.init()
window_dim = [256,240]
window = pygame.display.set_mode(window_dim,pygame.RESIZABLE|pygame.HWACCEL)
nes_surf = pygame.Surface((240,256))
p = pyaudio.PyAudio()

stream = p.open(format = p.get_format_from_width(2),channels=1,rate=44100,output=True,frames_per_buffer = 4096)
if len(sys.argv)>=2:
    nesObj = pyNES.NES(sys.argv[1])
else:
    nesObj = pyNES.NES(abspath("../../res/working_roms/helloworld.nes"))
controller_port1 = pyNES.Controller()
nesObj.setController(controller_port1,0)
cpu_mem = nesObj.cpuMem()
running = True
matches = np.arange(0,0x10000)
last_mem = deepcopy(cpu_mem)
def mem_s(ind):
    return str(cpu_mem[ind])

def tAudio():
    global running, nesObj, stream
    while running:
        c = nesObj.getAudio()
        stream.write(c)
  
def get_inputs():
    state = pygame.key.get_pressed()
    keys = [state[pygame.K_SPACE],
            state[pygame.K_LSHIFT],
            state[pygame.K_TAB],
            state[pygame.K_RETURN],
            state[pygame.K_UP],
            state[pygame.K_DOWN],
            state[pygame.K_LEFT],
            state[pygame.K_RIGHT]]
    controller_port1.updateInputs(keys)   
def draw():
    frame = deepcopy(nesObj.getImg())
    pygame.pixelcopy.array_to_surface(nes_surf,frame)
    scaled_ind = int(240/256<window_dim[1]/window_dim[0])
    scale_fac = [window_dim[0]/256,window_dim[1]/240][1-scaled_ind]
    nes_window_pos = [0,0]
    nes_window_pos[scaled_ind] = (window_dim[scaled_ind]-[256*scale_fac,240*scale_fac][scaled_ind])/2
    window.blit(pygame.transform.scale_by(pygame.transform.flip(pygame.transform.rotate(nes_surf,-90),True,False),scale_fac),nes_window_pos)
    
    
nesObj.perFrame(get_inputs)
audio_thread = threading.Thread(target=tAudio)
audio_thread.start()
world_num = 0
clock = pygame.time.Clock()
while running:
    nesObj.runFrame()
    draw()
    pygame.display.update()
    clock.tick(60)
    window.fill((0,0,0))
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            running = False
        elif event.type == pygame.VIDEORESIZE:
            window_dim = [event.w,event.h]
            window = pygame.display.set_mode(window_dim,pygame.RESIZABLE)
audio_thread.join()
stream.close()
p.terminate()