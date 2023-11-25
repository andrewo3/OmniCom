#include "../ppu.h"
#include "../cpu.h"

PPU::PPU(CPU* cpu) {
    linked = cpu;
    if (cpu->rom!=nullptr) {
        this->loadROM(cpu->rom);
    }
}

void PPU::loadROM(ROM* r) {
    
}

