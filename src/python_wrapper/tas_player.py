import sys
from os.path import abspath
sys.path.append(abspath("build/lib.macosx-10.9-universal2-cpython-312"))
sys.path.append(abspath("build\\lib.win-amd64-cpython-312"))
sys.path.append(abspath("build/lib.linux-x86_64-cpython-311"))
import pyNES,pygame, pyaudio, threading,time


pygame.init()
window_dim = [256,240]
window = pygame.display.set_mode(window_dim,pygame.RESIZABLE)
nes_surf = pygame.Surface((240,256))
p = pyaudio.PyAudio()

stream = p.open(format = p.get_format_from_width(2),channels=1,rate=44100,output=True,frames_per_buffer = 4096)

tas_file = open("C:\\Users\\Andrew Ogundimu\\Desktop\\HappyLee&Mars608_SMBwarpless_V5.fm2","r").readlines()
tas_inps = [list([i!="." for i in line.split("|")[2]]) for line in tas_file if line[0:2]=="|0"]
nesObj = pyNES.NES(abspath("../../res/working_roms/Super Mario Bros. (JU) [!].nes"))
nesObj.start()
running = True

def tAudio():
    global running, nesObj, stream
    while running:
        c = nesObj.getAudio()
        stream.write(c)

controller_port1 = pyNES.Controller()
nesObj.setController(controller_port1,0)

diff = 0

def tasInput():
    last = 0
    while running:
        frames = nesObj.frameCount()
        controller_port1.updateInputs(tas_inps[frames][::-1])
        print(tas_inps[frames][::-1])
        last = frames

audio_thread = threading.Thread(target=tAudio,daemon = True)
audio_thread.start()

tas_thread = threading.Thread(target=tasInput,daemon=True)
tas_thread.start()
clock = pygame.time.Clock()
while running:
    state = pygame.key.get_pressed()
    frame = nesObj.getImg()
    pygame.pixelcopy.array_to_surface(nes_surf,frame)
    scaled_ind = int(240/256<window_dim[1]/window_dim[0])
    scale_fac = [window_dim[0]/256,window_dim[1]/240][1-scaled_ind]
    nes_window_pos = [0,0]
    nes_window_pos[scaled_ind] = (window_dim[scaled_ind]-[256*scale_fac,240*scale_fac][scaled_ind])/2
    window.blit(pygame.transform.scale_by(pygame.transform.flip(pygame.transform.rotate(nes_surf,-90),True,False),scale_fac),nes_window_pos)
    pygame.display.update()
    window.fill((0,0,0))
    clock.tick(60)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            nesObj.stop()
            running = False
        elif event.type == pygame.VIDEORESIZE:
            window_dim = [event.w,event.h]
            window = pygame.display.set_mode(window_dim,pygame.RESIZABLE)
audio_thread.join()
tas_thread.join()
stream.close()
p.terminate()