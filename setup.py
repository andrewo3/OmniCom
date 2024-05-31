import os
import sys

print(sys.argv)
command = "python src/NES/python_wrapper/"+(" ".join(sys.argv))
print(command)
os.system(command)