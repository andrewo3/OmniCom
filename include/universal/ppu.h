#ifndef PPU_H
#define PPU_H

#include <cstdint>
#include <cstdbool>
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
        int8_t memory[0x4000]; // general memory
        int8_t oam[256]; // OAM (Object Attribute Memory) for sprites
        bool vram_twice = 0;
        int scanline = 261;
        int scycle = 0;
        bool debug = true;
        bool vblank = false;

        //rw
        int8_t read(int8_t* address);
        void write(int8_t* address, int8_t value);

        // registers
        uint16_t v = 0;
        uint16_t t = 0;
        uint8_t x = 0;
        uint8_t w = 0;

        uint16_t tile_addr;
        uint16_t attr_addr;

        uint16_t pthigh; //pattern table high bit data
        uint16_t ptlow; //pattern table low bit data
        uint8_t pattern;
    private:
        void map_memory(int8_t** addr);
        uint16_t get_addr(int8_t* ptr);
        void apply_and_update_registers();
        uint16_t upcoming_pattern;
        uint8_t internalx;
        void v_horiz();
        void v_vert();
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