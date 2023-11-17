#include "cpu.h"
#include <stdio.h>

int main(int argc, char ** argv) {
    ROM rom(argv[1]);
    if (!rom.is_valid()) {
        printf("Usage: nes rom_filename");
        return -1;
    }
    CPU cpu;
    return 0;
}