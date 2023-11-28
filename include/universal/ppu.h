#ifndef PPU_H
#define PPU_H

#include <cstdint>
#include "rom.h"

class CPU;

class PPU {
    public:
        PPU();
        PPU(CPU* c);
        void cycle();
        CPU* cpu;
        void loadRom(ROM* r);
        void set_registers();
        ROM* rom;
        long long cycles = 0; // total cycles
    private:
        int8_t memory[0x4000]; // general memory
        int8_t oam[256]; // OAM (Object Attribute Memory) for sprites
        void map_memory();
        int scanline;
        int scycle = 0;
        // registers
        int8_t* PPUCTRL; //&memory[0x2000]
        int8_t* PPUMASK; //&memory[0x2001]
        int8_t* PPUSTATUS; //&memory[0x2002]
        int8_t* OAMADDR; //&memory[0x2003]
        int8_t* OAMDATA; //&memory[0x2004]
        int8_t* PPUSCROLL; //&memory[0x2005]
        int8_t* PPUADDR; //&memory[0x2006]
        int8_t* PPUDATA; //&memory[0x2007]
        int8_t* OAMDMA; //&memory[0x4014]
};

#endif