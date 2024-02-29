from glob import glob
from sys import platform
from os import environ, chdir
from os.path import *
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension
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
root_src = glob("../*.cpp")
imgui = glob("../imgui/*.cpp")
imgui_backends = glob("../imgui/backend/*.cpp")
files = sorted(root_src+imgui+imgui_backends)
files.append("wrapper.cpp")
files.remove(f"..{file_sep}main.cpp")
libs = sorted(glob("../../lib/*.*"))
include_path = f"{realpath(f"../../include/{folder}")}{sep}{realpath("../ntsc-filter")}{sep}{realpath("../../include/universal")}{sep}{realpath("../imgui")}{sep}{realpath("../imgui/backends")}"
ext_modules = [
    Pybind11Extension(
        "pyNES",
        files
    ),
]

environ["CFLAGS"] = "-std=c++11"
if platform == "win32":
    environ["INCLUDE"] = include_path
    environ["LIB"] = realpath("../../lib")
else:
    environ["CFLAGS"]+=f" -F{realpath("../../bin")} -framework SDL2 -rpath {realpath("../../bin")}"
    environ["CPLUS_INCLUDE_PATH"] += include_path
    environ["LD_LIBRARY_PATH"] = realpath("../../lib")

setup(
    name='pyNES',
    version='0.0.1',
    ext_modules=ext_modules
)