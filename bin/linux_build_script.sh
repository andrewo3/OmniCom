xxd -n fragment -i res/shaders/main.frag > include/universal/shader_data.h
xxd -n vertex -i res/shaders/main.vert >> include/universal/shader_data.h
g++ src/ntsc-filter/crt_core.c src/ntsc-filter/crt_ntsc.c src/util.cpp src/rom.cpp src/cpu.cpp src/cpu_helper.cpp src/mapper.cpp src/ppu.cpp src/apu.cpp -g src/main.cpp -Isrc/ntsc-filter -Iinclude/universal -lSDL2 -lGL -lGLEW -lGLU -o "bin/main"