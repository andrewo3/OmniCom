import os
import sys

print(sys.argv)
command = "python "+(" ".join(sys.argv)).replace("setup.py","src/NES/python_wrapper/setup.py")
print(command)
os.system(command)