# Nes2Exec

[Project Video](https://youtu.be/7l0gG79EPss)

Using (the poorly named) Nes2Exec program is simple. First, [SDL2](https://wiki.libsdl.org/SDL2/Installation), [OpenGL](https://www.khronos.org/opengl/wiki/Getting_Started#Downloading_OpenGL),  and [GLEW](https://glew.sourceforge.net/) need to be available and usable on your system. If using Mac, I recommend installing SDL2 and GLEW with [homebrew](https://brew.sh/). It's really easy to download (one command), and it just works out of the box. I packaged the libraries and headers within this project, but in my testing it seems sometimes some computers just don't detect them and instead look in system directories, so make sure they are installed. 

But if all goes well you actually shouldn't even need these downloads.

Next, to compile the program, cd to this project directory and run the following command, in accordance with your system (and install [gcc](https://gcc.gnu.org/releases.html) if it is not present):

## Mac
```g++ -v src/util.cpp src/rom.cpp src/cpu.cpp src/cpu_helper.cpp src/ppu.cpp src/apu.cpp -g src/main.cpp -I./include/universal -I./include/unix -L./lib -F./bin -framework SDL2 -framework OpenGL -lGLEW -o bin/main```

## Windows + Linux
```g++ src/util.cpp src/rom.cpp src/cpu.cpp src/cpu_helper.cpp src/ppu.cpp src/apu.cpp -g src/main.cpp -Iinclude/universal -lSDL2 -lGL -lGLEW -lGLU -o bin/main```

In theory, these commands should also work with clang but I haven't tried it myself.

Once you've compiled - Using the program is as easy as running ```bin/main```, followed by the path to any ROM you want to play.

At the moment none of the roms have audio because I couldn't get it working in time.

For all ROMs, the controls are as follows:
|NES | Keyboard|
| --- | --- |
|A | Space|
|B | Left Shift|
|Select | Tab |
| Start | Enter |
|Up | Up Arrow |
| Down | Down Arrow |
| Left | Left Arrow |
| Right | Right Arrow |

This is the configuration I found works well and makes the most sense for a lot of games.

Most ROMs in the res/roms folder do not work, so I made a (small) folder called res/working_roms with the roms I tested and that I know work.

Happy emulating!