xxd -n fragment -i res/shaders/main.frag > include/universal/shader_data.h
xxd -n vertex -i res/shaders/main.vert >> include/universal/shader_data.h
g++ -Isrc/imgui -Isrc/imgui/backends -Iinclude/unix -Isrc/ntsc-filter -Iinclude/universal src/imgui/backends/*.cpp src/imgui/*.cpp src/ntsc-filter/crt_core.c src/ntsc-filter/crt_ntsc.c src/*.cpp -lSDL2 -lGL -lGLEW -lGLU -o "bin/main"