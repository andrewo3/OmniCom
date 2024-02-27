#include "cpu.h"
#include "ppu.h"
#include "rom.h"
#include "util.h"
#include "mapper.h"
#include "apu.h"
#include <cstdint>

class CPU;
class PPU;
class APU;
class ROM;

class NES {
    public:
        NES(char* rom_name);
        int mapper;
        uint8_t* cpu_mem;
        uint8_t* ppu_mem;
    private:
        CPU* cpu;
        PPU* ppu;
        APU* apu;
        ROM* rom;
};

NES::NES(char* rom_name) {
    rom = new ROM(rom_name);
    cpu = new CPU();
    ppu = new PPU(cpu);
    apu = new APU();
}

NES::~NES() {
    delete rom;
    delete cpu;
    delete ppu;
    delete apu;
}

