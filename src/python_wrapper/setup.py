from glob import glob
from os import environ
from os.path import *
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension

#sorted(glob("*.cpp")),  # Sort source files for reproducibility
ext_modules = [
    Pybind11Extension(
        "pyNES",
        ["wrapper.cpp"]
    ),
]

include_path = f"{abspath("..\\..\\include\\unix")};{abspath("..\\..\\include\\win32")};{abspath("..\\ntsc-filter")};{abspath("..\\..\\include\\universal")};{abspath("..\\imgui")};{abspath("..\\imgui\\backends")}"

environ["INCLUDE"] = include_path
environ["LIB"] = abspath("..\\..\\lib")

setup(
    name='pyNES',
    version='0.0.1',
    ext_modules=ext_modules
)