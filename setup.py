import os
import sys

print(sys.argv)
command = f"python src/NES/python_wrapper/{" ".join(sys.argv)}"
print(command)
os.system(command)