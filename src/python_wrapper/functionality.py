import sys
from os.path import abspath
sys.path.append(abspath("build/lib.macosx-10.9-universal2-cpython-312"))
import pyNES,pygame

nesObj = pyNES.NES(abspath("../../res/roms/Super Mario Bros. 3 (U) (PRG1) [!].nes"))
#nesObj = pyNES.NES(abspath("../../res/working_roms/helloworld2.nes"))
print(dir(nesObj))
nesObj.start()

controller_port1 = pyNES.Controller()
nesObj.setController(controller_port1,0)
cpu_mem = nesObj.cpuMem()

window_dim = [256,240]
window = pygame.display.set_mode(window_dim,pygame.RESIZABLE)
nes_surf = pygame.Surface((240,256))
running = True
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
    controller_port1.updateInputs(keys)
    cpu_mem[0x736] = 10
    pygame.pixelcopy.array_to_surface(nes_surf,nesObj.getImg())
    window.blit(pygame.transform.scale(pygame.transform.flip(pygame.transform.rotate(nes_surf,-90),True,False),window_dim),(0,0))
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
            