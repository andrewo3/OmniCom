#include "cpu.h"
#include <cstdio>

using namespace SNES;

CPU::CPU() {
    
}

uint8_t CPU::read(uint32_t addr) {
    uint32_t rom_addr = rom->map(addr);
    return rom->mem[rom_addr];
}

void CPU::clock() {
    printf("CPU CLOCK\n");
    auto located = func_cache.find(PC);
    if (located==func_cache.end()) { //if function is not in cache,
        //build new function



    } else { // execute function from cache
        located->second();
    }
    printf("val at PC: %06x->%02x\n",PC&0xffffff,read(rom->map(PC)));
    PC++;
}

void CPU::reset() {
    printf("CPU RESET\n");
    uint16_t* reset_vec = (uint16_t*)((rom->mem)+rom->map(0xfffc));
    this->PC = *reset_vec;
    this->SP = 0x1ff;
    printf("RESET: %04x->%04x\n",*reset_vec,rom->map(*reset_vec));
}