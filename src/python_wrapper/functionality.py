import sys
from os.path import abspath
sys.path.append(abspath("build/lib.macosx-10.9-universal2-cpython-312"))
import pyNES

nesObj = pyNES.NES(abspath("../../res/roms/Super Mario Bros. 3 (U) (PRG1) [!].nes"))
print("Works!")