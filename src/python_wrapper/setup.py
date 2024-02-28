from glob import glob
from sys import platform
from os import environ, chdir
from os.path import *
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension

cwd = dirname(realpath(__file__))
chdir(cwd)
#sorted(glob("*.cpp")),  # Sort source files for reproducibility
files = sorted(glob("../*.cpp"))
files.append("wrapper.cpp")
files.remove("../main.cpp")
libs = sorted(glob("../../lib/*.*"))
ext_modules = [
    Pybind11Extension(
        "pyNES",
        files,
        
    ),
]
sep = ":"
if platform == "win32":
    sep = ";"

include_path = f"{realpath("../../include/unix")}{sep}{realpath("../../include/win32")}{sep}{realpath("../ntsc-filter")}{sep}{realpath("../../include/universal")}{sep}{realpath("../imgui")}{sep}{realpath("../imgui/backends")}"

if platform == "win32":
    environ["INCLUDE"] = include_path
    environ["LIB"] = realpath("../../lib")
else:
    environ["CFLAGS"]="-std=c++11"
    environ["CPLUS_INCLUDE_PATH"] += include_path
    environ["LD_LIBRARY_PATH"] = realpath("../../lib")

setup(
    name='pyNES',
    version='0.0.1',
    ext_modules=ext_modules
)