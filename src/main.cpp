#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#include "rom.h"
#include "cpu.h"
#include "ppu.h"
#include <cstdio>
#include <string>
#include <csignal>
#include <chrono>
#include <thread>
#include <mutex>

std::mutex interruptedMutex;

const int NES_DIM[2] = {256,240};
const int FLAGS = SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE;

volatile sig_atomic_t interrupted = 0;

long long total_ticks;
long long start;

int usage_error() {
    printf("Usage: nes rom_filename\n");
    return -1;
}

int invalid_error() {
    printf("Invalid NES rom!\n");
    return -1;
}

void get_filename(char** path) {
    int l = strlen(*path);
    for (int i=l-1; i>=0; i--) {
        if ((*path)[i]=='.') {
            (*path)[i] = '\0';
        } else if ((*path)[i]=='/' || (*path)[i]=='\\') {
            *path = &(*path)[i+1];
            return;
        }
    }
}

// get time in milliseconds since epoch
long long epoch() {
    auto currentTimePoint = std::chrono::system_clock::now();
    auto durationSinceEpoch = currentTimePoint.time_since_epoch();
    auto millisecondsSinceEpoch = std::chrono::duration_cast<std::chrono::milliseconds>(durationSinceEpoch);
    return millisecondsSinceEpoch.count();
}

void quit(int signal) {
    std::lock_guard<std::mutex> lock(interruptedMutex);
    printf("MIPS: %lf - Target: (approx.) 0.43\n",total_ticks/(epoch()-start)/1000.0);
    interrupted = 1;
}

void NESLoop(ROM* r_ptr) {
    printf("Mapper: %i\n",r_ptr->get_mapper()); //https://www.nesdev.org/wiki/Mapper
    CPU cpu(false);
    printf("CPU Initialized.\n");
    cpu.loadRom(r_ptr);
    PPU ppu(&cpu);
    printf("PPU Initialized\n");
    printf("ROM loaded into CPU.\n");
    //emulator loop
    while (!interrupted) {
        cpu.clock();
        ppu.clock();
        total_ticks = cpu.clocks;
    }
}

int main(int argc, char ** argv) {
    std::signal(SIGINT,quit);
    start = epoch();
    if (argc!=2) {
        return usage_error();
    }
    ROM rom(argv[1]);
    if (!rom.is_valid()) {
        return invalid_error();
    }
    char* filename = new char[strlen(argv[1])];
    memcpy(filename,argv[1],strlen(argv[1]));
    get_filename(&filename);
    // SDL initialize
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
    SDL_Window* window = SDL_CreateWindow(filename,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,NES_DIM[0],NES_DIM[1],FLAGS);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_Event event;
    //Initialize everything else and enter NES loop alongside window loop
    std::thread NESThread(NESLoop,&rom);
    //main window loop
    while (!interrupted) {
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    quit(0);
                    break;
            }
        }
    }
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    NESThread.join();
    return 0;
}