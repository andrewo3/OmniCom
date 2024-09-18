import sys, random, time, pyaudio, threading
from datetime import datetime
from copy import deepcopy
import numpy as np
from io import BytesIO
from requests import get
import omnicom,pygame

pygame.init()
window_dim = [256,240]
window = pygame.display.set_mode(window_dim,pygame.RESIZABLE)
nes_surf = pygame.Surface((240,256))
p = pyaudio.PyAudio()

stream = p.open(format = p.get_format_from_width(2),channels=1,rate=44100,output=True,frames_per_buffer = 2048)
#nesObj = omnicom.NES(abspath("../../res/working_roms/Super Mario Bros. 3 (U) (PRG1) [!].nes"))
#nesObj = omnicom.NES("C:\\Users\\Andrew Ogundimu\\Desktop\\smb3mix-rev2B-prg1.nes")
if len(sys.argv)>=2:
    nesObj = omnicom.NES(sys.argv[1])
else:
    url ="https://archive.org/download/pacman_nes_2/pacman.nes"

    rom = BytesIO(get(url).content)
    rom.name = url.split("/")[-1]
    nesObj = omnicom.NES(rom)
#nesObj = omnicom.NES(abspath("../../res/working_roms/Super Mario Bros. (JU) [!].nes"))
#nesObj = omnicom.NES(abspath("../../res/working_roms/helloworld2.nes"))
controller_port1 = omnicom.Controller()
nesObj.setController(controller_port1,0)
cpu_mem = nesObj.cpuMem()
running = True
matches = np.arange(0,0x10000)
last_mem = deepcopy(cpu_mem)
def mem_s(ind):
    return str(cpu_mem[ind])
nesObj.start()

def tAudio():
    global running, nesObj, stream
    while running:
        c = nesObj.getAudio()
        stream.write(c)

frame = deepcopy(nesObj.getImg())

def frameUpdate():
    global frame
    #nesObj.setPaused(True)
    frame = deepcopy(nesObj.getImg())
    #nesObj.setPaused(False)

audio_thread = threading.Thread(target=tAudio)
audio_thread.start()
world_num = 0
#nesObj.perFrame(frameUpdate)

def set_score(i):
    i//=10
    d = 0
    p = []
    while (i > 0):
        p.append(i%256)
        i//=256
    p.append(0)
    p.append(0)
    p.append(0)
    p.append(0)
    cpu_mem[0x715:0x718] = p[:3][::-1]

while running:
    state = pygame.key.get_pressed()
    keys = [state[pygame.K_SPACE],
            state[pygame.K_LSHIFT],
            state[pygame.K_TAB],
            state[pygame.K_RETURN],
            state[pygame.K_UP],
            state[pygame.K_DOWN],
            state[pygame.K_LEFT],
            state[pygame.K_RIGHT]]
    score_str = ""
    for i in [112,113,114,115,116][::-1]:
        score_str+=str(cpu_mem[i])
    score_str+="0"
    #pygame.display.set_caption(str(cpu_mem[0x736]))
    #cpu_mem[0x736] = 99
    t = datetime.now()
    score = int(str(t.hour).zfill(2)+str(t.minute).zfill(2)+str(t.second).zfill(2))*10
    #set_score(score)
    nesObj.setPaused(state[pygame.K_p])
    controller_port1.updateInputs(keys)
    frame = deepcopy(nesObj.getImg())
    pygame.pixelcopy.array_to_surface(nes_surf,frame)
    scaled_ind = int(240/256<window_dim[1]/window_dim[0])
    scale_fac = [window_dim[0]/256,window_dim[1]/240][1-scaled_ind]
    nes_window_pos = [0,0]
    nes_window_pos[scaled_ind] = (window_dim[scaled_ind]-[256*scale_fac,240*scale_fac][scaled_ind])/2
    window.blit(pygame.transform.scale_by(pygame.transform.flip(pygame.transform.rotate(nes_surf,-90),True,False),scale_fac),nes_window_pos)
    pygame.display.update()
    window.fill((0,0,0))
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            running = False
            print("running stop")
        elif event.type == pygame.VIDEORESIZE:
            window_dim = [event.w,event.h]
            window = pygame.display.set_mode(window_dim,pygame.RESIZABLE)
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_d:
                nesObj.setPaused(True)
                tmp_mem = deepcopy(cpu_mem)
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
nesObj.stop()
audio_thread.join()
stream.close()
p.terminate()