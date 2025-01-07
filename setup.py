from glob import glob
from sys import platform
import sys
sys.path.append("pybind11")
from os import environ, chdir, system, getcwd,listdir
from os.path import *
from shutil import rmtree
import setuptools

print("setuptools version:",setuptools.__version__)

file_sep = "/"
sep = ":"
folder = "unix"
if platform == "win32":
    file_sep = "\\"
    sep = ";"
    folder = "win32"

#sorted(glob("*.cpp")),  # Sort source files for reproducibility
root = f"."
print(getcwd(),root)
root_src = glob(f"src{file_sep}NES{file_sep}*.cpp")
files = sorted(root_src)
files.append(f"python_wrapper{file_sep}wrapper.cpp")
files.append(f"src{file_sep}glob_const.cpp")
#files.remove(f"{root}{file_sep}src{file_sep}NES{file_sep}nes_sys.cpp")
#files.remove(f"{root}{file_sep}src{file_sep}util.cpp")
lib_path = realpath(f"{root}{file_sep}lib")
libs = sorted(glob(f"{lib_path}{file_sep}*.*"))
include_path = [f'{root}{file_sep}include{file_sep}universal',f'{root}{file_sep}src{file_sep}NES']
library_paths = []
library_paths.append(lib_path)
libraries = []

environ["CFLAGS"] = "-std=c++17"
if platform == "darwin":
    environ["CFLAGS"]+=" -mmacosx-version-min=10.15"
if platform == "win32":
    environ["CL"] = "/std:c++17"
    #environ["INCLUDE"] = include_path
    environ["LIBPATH"] = realpath(f"{root}{file_sep}lib")
    libraries.append("shell32")
    #environ["CFLAGS"]+=f" -L{realpath("..\\..\\lib")} -rpath {realpath("..\\..\\lib")} -lmingw32 -lSDL2main -lSDL2 -mwindows"
else:
    #environ["CFLAGS"]+=f" -F{realpath('../../bin')} -framework SDL2 -rpath {realpath('../../bin')}"
    try:
        environ["CPLUS_INCLUDE_PATH"] += f"{sep}".join(include_path)
    except KeyError:
        pass
    environ["LD_LIBRARY_PATH"] = realpath(f"{root}{file_sep}lib")

from setuptools import setup
try:
    from pybind11.setup_helpers import Pybind11Extension
    print("PYBIND11 FOUND")
except ImportError:
    from setuptools import Extension as Pybind11Extension
    print("PYBIND11 NOT FOUND")

ext_modules = [
    Pybind11Extension(
        "omnicom",
        sources = files,
        include_dirs = include_path,
        libraries=libraries,
        extra_compile_args=['-std=c++17']
    )
]

stubs = f"python_wrapper{file_sep}omnicom.pyi"
from setuptools.command.build_ext import build_ext

class CopyStubs(build_ext):
    def run(self):
        # Run the original build process
        print("Run original build")
        super().run()
        print("Finished main build script")
        # Copy the .pyi file into the build directory
        self.copy_pyi_file()

    def copy_pyi_file(self):
        global stubs
        # Source location of the .pyi file
        source = stubs

        # Copy the .pyi file
        target = join(self.build_lib, "omnicom.pyi")
        print(f"Copying Stubs: {source} to {target}")
        with open(stubs,'r') as s:
            stubs_text = s.read()
        with open(target,'w') as s_copy:
            s_copy.write(stubs_text)

setup(
    cmdclass = {"build_ext": CopyStubs},
    name='omnicom',
    version='0.3.6',
    ext_modules=ext_modules
)