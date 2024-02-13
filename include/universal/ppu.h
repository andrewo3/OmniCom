#ifndef PPU_H
#define PPU_H

#include <cstdint>
#include <cstdbool>
#include <mutex>
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
        long long frames = 0;
        int8_t memory[0x4000]; // general memory
        int8_t chr_ram[0x8000]; //chr-ram (used by some mappers)
        int8_t oam[256]; // OAM (Object Attribute Memory) for sprites
        int8_t secondary_oam[32]; //sprites to draw on each scanline.
        bool vram_twice = 0;
        int scanline = 261;
        int scycle = 0;
        bool debug = false;
        bool vblank = false;
        std::mutex image_mutex;
        bool mutex_locked = false;
        int chr_bank_num = 0;


        //test
        int vbl_count = 0;

        //rw
        int8_t read(int16_t address);
        void write(int16_t address, int8_t value);
        void v_horiz();
        void v_vert();

        uint16_t address_bus = 0;

        // registers
        uint16_t v = 0;
        uint16_t t = 0;
        uint8_t x = 0;
        uint8_t w = 0;

        uint8_t oam_addr;
        uint16_t tile_addr;
        uint16_t attr_addr;

        uint16_t pthigh; //pattern table high bit data
        uint16_t ptlow; //pattern table low bit data
        uint8_t pattern;
        uint8_t read_buffer = 0;

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
    private:
        void map_memory(int16_t* addr);
        long long get_addr(int8_t* ptr);
        void apply_and_update_registers();
        uint16_t upcoming_pattern;
        uint8_t internalx;

        //sprite stuff
        uint8_t sprite_eval_n;
        uint8_t sprite_eval_m;
        bool sprite_eval_end;
        uint8_t sprites;
        int8_t scanlinesprites[32] = {0};
        int8_t scanlinespritenum = 0;
        int sprite_x_counters[8] = {0};
        int next_sprite_x_counters[8] = {0};
        uint8_t active_sprites = {0}; //8 bits - each bit will represent one sprite.
        uint8_t sprite_patterns[8] = {0};
        bool spritezeropresent = false;
        bool nextspritezeropresent = false;
        bool sprite_eval = false;
        unsigned char internal_img[184320]; //output image running copy
};

#endif