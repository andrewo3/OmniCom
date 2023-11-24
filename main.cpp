#include "hardware/rom.h"
#include "hardware/cpu.h"
#include "hardware/ppu.h"
#include <cstdio>
#include <csignal>
#include <chrono>

long long total_ticks;
long long start;

volatile sig_atomic_t interrupted = 0;

int usage_error() {
    printf("Usage: nes rom_filename\n");
    return -1;
}

int invalid_error() {
    printf("Invalid NES rom!\n");
    return -1;
}

// get time in milliseconds since epoch
long long epoch() {
    auto currentTimePoint = std::chrono::system_clock::now();
    auto durationSinceEpoch = currentTimePoint.time_since_epoch();
    auto millisecondsSinceEpoch = std::chrono::duration_cast<std::chrono::milliseconds>(durationSinceEpoch);
    return millisecondsSinceEpoch.count();
}

void exit(int signal) {
    printf("MIPS: %lf\n",total_ticks/(epoch()-start)*1000/(1000000.0));
    interrupted = 1;
}

int main(int argc, char ** argv) {
    std::signal(SIGINT,exit);
    try {
        start = epoch();
        if (argc!=2) {
            return usage_error();
        }
        ROM rom(argv[1]);
        if (!rom.is_valid()) {
            return invalid_error();
        }
        printf("Mapper: %i\n",rom.get_mapper()); //https://www.nesdev.org/wiki/Mapper
        CPU cpu(false);
        printf("CPU Initialized.\n");
        PPU ppu;
        printf("PPU Initialized\n");
        cpu.loadRom(&rom);
        printf("ROM loaded into CPU.\n");
        while (!interrupted) {
            cpu.clock();
            total_ticks = cpu.clocks;
        }
    } catch (...) {
        printf("An exception occurred.\n");
        return -1;
    }
    return 0;
}