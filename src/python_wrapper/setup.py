from glob import glob
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension

ext_modules = [
    Pybind11Extension(
        "test",
        sorted(glob("*.cpp")),  # Sort source files for reproducibility
    ),
]

setup(
    name='test',
    version='0.0.1',
    ext_modules=ext_modules
)