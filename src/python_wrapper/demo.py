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
window = pygame.display.set_mode(window_dim,pygame.RESIZABLE)
nes_surf = pygame.Surface((240,256))
p = pyaudio.PyAudio()

stream = p.open(format = p.get_format_from_width(2),channels=1,rate=44100,output=True,frames_per_buffer = 4096)

#nesObj = pyNES.NES(abspath("../../res/working_roms/Super Mario Bros. 3 (U) (PRG1) [!].nes"))
#nesObj = pyNES.NES("C:\\Users\\Andrew Ogundimu\\Desktop\\smb3mix-rev2B-prg1.nes")
if len(sys.argv)>=2:
    nesObj = pyNES.NES(sys.argv[1])
else:
    nesObj = pyNES.NES(abspath("../../res/working_roms/Tetris (U) [!].nes"))
#nesObj = pyNES.NES(abspath("../../res/working_roms/Super Mario Bros. (JU) [!].nes"))
#nesObj = pyNES.NES(abspath("../../res/working_roms/helloworld2.nes"))
print("SAVEDIR:",nesObj.getSaveDir())
success = nesObj.setSaveDir(abspath("./states"))
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

def tAudio():
    global running, nesObj, stream
    while running:
        c = nesObj.getAudio()
        stream.write(c)

audio_thread = threading.Thread(target=tAudio)
audio_thread.start()
world_num = 0
saves = 2
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
    nesObj.setPaused(state[pygame.K_p])
    pygame.display.set_caption("World: "+str(cpu_mem[0x727]+1)+", Lives: "+str(cpu_mem[0x736])) #[ 1894  1917  1918 32723 32724] 5
    #cpu_mem[0x75F]=world_num
    cpu_mem[0x47] = world_num
    #cpu_mem[0x7D00:0x7D40] = np.zeros(0x40)
    #keys = random.choices([0,1],k=8)
    #keys[3] = state[pygame.K_RETURN]
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
            elif event.unicode and 0<=(ord(event.unicode)-ord('0'))<=9:
                num = ord(event.unicode)-ord('0')
                nesObj.setPaused(True)
                if nesObj.loadState(num):
                    print("Successfully loaded state",num)
                else:
                    print("Unable to load state",num)
                nesObj.setPaused(False)
            elif event.key == pygame.K_d:
                type = int(input(
"""Type of search: 
0 - reset memory table
1 - exact value
2 - changed (since last search)
3 - unchanged (since last search)
4 - increased (since last search)
5 - decreased (since last search)
"""))
                tmp_mem = deepcopy(cpu_mem)
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
audio_thread.join()
stream.close()
p.terminate()