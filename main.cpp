#include "rom.h"
#include "cpu.h"
#include <stdio.h>

int usage_error() {
    printf("Usage: nes rom_filename\n");
        return -1;
}

int invalid_error() {
    printf("Invalid NES rom!\n");
    return -1;
}

int main(int argc, char ** argv) {
    if (argc!=2) {
        return usage_error();
    }
    ROM rom(argv[1]);
    if (!rom.is_valid()) {
        return invalid_error();
    }
    printf("%i\n",rom.get_mapper());
    CPU cpu;
    printf("CPU Initialized.\n");
    cpu.loadRom(&rom);
    printf("ROM loaded into CPU.\n");
    while (true) {
        cpu.clock();
    }
    return 0;
}