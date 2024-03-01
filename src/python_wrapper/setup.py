from glob import glob
from sys import platform
from os import environ, chdir
from os.path import *
file_sep = "/"
sep = ":"
folder = "unix"
if platform == "win32":
    file_sep = "\\"
    sep = ";"
    folder = "win32"

cwd = dirname(realpath(__file__))
chdir(cwd)
#sorted(glob("*.cpp")),  # Sort source files for reproducibility
root_src = glob(f"{realpath("..")}"+f"{file_sep}*.cpp")
imgui = glob(f"..{file_sep}imgui{file_sep}*.cpp")
imgui_backends = glob(f"..{file_sep}imgui{file_sep}backend{file_sep}*.cpp")
files = sorted(root_src+imgui+imgui_backends)
files.append("wrapper.cpp")
files.remove(realpath(f"..{file_sep}main.cpp"))
lib_path = realpath(f"..{file_sep}..{file_sep}lib")
libs = sorted(glob(f"{lib_path}{file_sep}*.*"))
include_path = f"{realpath(f"../../include/{folder}".replace("/",file_sep))}{sep}{realpath("../ntsc-filter".replace("/",file_sep))}{sep}{realpath("../../include/universal".replace("/",file_sep))}{sep}{realpath("../imgui".replace("/",file_sep))}{sep}{realpath("../imgui/backends".replace("/",file_sep))}"
include_path = include_path.replace("/",file_sep)

library_paths = []
library_paths.append(lib_path)

environ["CFLAGS"] = "-std=c++11"
if platform == "win32":
    #environ["INCLUDE"] = include_path
    environ["LIBPATH"] = realpath("..\\..\\lib")
    #environ["CFLAGS"]+=f" -L{realpath("..\\..\\lib")} -rpath {realpath("..\\..\\lib")} -lmingw32 -lSDL2main -lSDL2 -mwindows"
else:
    environ["CFLAGS"]+=f" -F{realpath("../../bin")} -framework SDL2 -rpath {realpath("../../bin")}"
    environ["CPLUS_INCLUDE_PATH"] += include_path
    environ["LD_LIBRARY_PATH"] = realpath("../../lib")

from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension

ext_modules = [
    Pybind11Extension(
        "pyNES",
        sources = files,
        include_dirs = include_path.split(sep)
    )
]

setup(
    name='pyNES',
    version='0.0.1',
    ext_modules=ext_modules
)