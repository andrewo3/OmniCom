#include "cpu.h"
#include <stdio.h>

using namespace SNES;

CPU::CPU() {
    
}

uint8_t CPU::read(uint32_t addr) {
    uint32_t rom_addr = rom->map(addr);
    return rom->mem[rom_addr];
}

void CPU::clock() {
    //printf("CPU CLOCK\n");
    printf(read(PC));
    PC++;
}

void CPU::reset() {
    printf("CPU RESET\n");
    uint16_t* reset_vec = (uint16_t*)((rom->mem)+rom->map(0xfffc));
    this->PC = *reset_vec;
    printf("RESET: %04x->%04x\n",*reset_vec,rom->map(*reset_vec));
}