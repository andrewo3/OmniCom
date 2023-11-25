#include <cstdint>
#include "cpu.h"

class PPU {
    public:
        PPU();
        PPU(CPU* cpu);
        int8_t* PPUCTRL; //&memory[0x2000]
        int8_t* PPUMASK; //&memory[0x2001]
        int8_t* PPUSTATUS; //&memory[0x2002]
        int8_t* OAMADDR; //&memory[0x2003]
        void render();
        CPU* linked;
        void loadROM(ROM* r);
        ROM* rom;
    private:
        int8_t memory[0x4000];
        void map_memory();
};