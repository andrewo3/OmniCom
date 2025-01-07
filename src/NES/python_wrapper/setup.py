from glob import glob
from sys import platform
from os import environ, chdir, system, getcwd,listdir
from os.path import *
file_sep = "/"
sep = ":"
folder = "unix"
if platform == "win32":
    file_sep = "\\"
    sep = ";"
    folder = "win32"

print(getcwd(),listdir(),listdir('..'))  
cwd = dirname(realpath(__file__))
chdir(cwd)
#sorted(glob("*.cpp")),  # Sort source files for reproducibility
root = f"..{file_sep}..{file_sep}.."
print(root)
root_src = glob(f"..{file_sep}*.cpp")
print(cwd,"src files:",root_src)
files = sorted(root_src)
files.append("wrapper.cpp")
files.append(f"{root}{file_sep}src{file_sep}glob_const.cpp")
files.remove(f"..{file_sep}nes_sys.cpp")
print(files)
#files.remove(f"{root}{file_sep}src{file_sep}util.cpp")
lib_path = realpath(f"{root}{file_sep}lib")
libs = sorted(glob(f"{lib_path}{file_sep}*.*"))
include_path = f"{realpath(f'{root}{file_sep}include{file_sep}{folder}')}\
{sep}\
{realpath(f'{root}{file_sep}include{file_sep}universal')}"

include_path = include_path.replace("/",file_sep)

library_paths = []
library_paths.append(lib_path)
libraries = []

environ["CFLAGS"] = "-std=c++17 -mmacosx-version-min=10.15"
if platform == "win32":
    environ["CL"] = "/std:c++17"
    #environ["INCLUDE"] = include_path
    environ["LIBPATH"] = realpath("..\\..\\lib")
    libraries.append("shell32")
    #environ["CFLAGS"]+=f" -L{realpath("..\\..\\lib")} -rpath {realpath("..\\..\\lib")} -lmingw32 -lSDL2main -lSDL2 -mwindows"
else:
    #environ["CFLAGS"]+=f" -F{realpath('../../bin')} -framework SDL2 -rpath {realpath('../../bin')}"
    try:
        environ["CPLUS_INCLUDE_PATH"] += include_path
    except KeyError:
        pass
    environ["LD_LIBRARY_PATH"] = realpath("../../lib")

from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension

ext_modules = [
    Pybind11Extension(
        "omnicom",
        sources = files,
        include_dirs = include_path.split(sep),
        libraries=libraries
    )
]


setup(
    name='omnicom',
    version='0.3.0',
    ext_modules=ext_modules,
    py_modules=["omnicom"],
    package_data={"omnicom":["*.pyi"]},
    include_package_data=True
)