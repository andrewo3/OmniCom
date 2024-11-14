#include "cpu.h"
#include <stdio.h>

using namespace SNES;

CPU::CPU() {
    
}

void CPU::clock() {
    //printf("CPU CLOCK\n");
    //memory[PC];
}

void CPU::reset() {
    printf("CPU RESET\n");
    uint16_t* reset_vec = (uint16_t*)((rom->mem)+rom->map(0xfffc));
    printf("RESET: %04x->%04x\n",*reset_vec,rom->map(*reset_vec));
}