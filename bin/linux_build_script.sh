xxd -n fragment -i res/shaders/main.frag > include/universal/shader_data.h
xxd -n vertex -i res/shaders/main.vert >> include/universal/shader_data.h
g++ src/rom.cpp src/cpu.cpp src/cpu_helper.cpp src/ppu.cpp -g src/main.cpp -Iinclude/universal -lSDL2 -lGL -lGLEW -o "/WIN_D/Projects/Visual Studio Code/Nes2Execv2/bin/main"