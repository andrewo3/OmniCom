#include "ppu.h"
#include "cpu.h"
#include <cstring>

PPU::PPU() {
    this->scanline = 0;
    this->set_registers();
}

void PPU::set_registers() {
    this->PPUCTRL = &(cpu->memory[0x2000]);
    this->PPUMASK = &(cpu->memory[0x2001]);
    this->PPUSTATUS = &(cpu->memory[0x2002]);
    this->OAMADDR = &(cpu->memory[0x2003]);
    this->OAMDATA = &(cpu->memory[0x2004]);
    this->PPUSCROLL = &(cpu->memory[0x2005]);
    this->PPUADDR = &(cpu->memory[0x2006]);
    this->PPUDATA = &(cpu->memory[0x2007]);
    this->OAMDMA = &(cpu->memory[0x4014]);
}

PPU::PPU(CPU* c) {
    scanline = 0;
    cpu = c;
    cpu->ppu = this;
    if (cpu->rom!=nullptr) {
        this->loadRom(cpu->rom);
        this->set_registers();
        
    }
}

void PPU::cycle() {
    if (0<=scanline && scanline<=239) { // visible scanlines
        if (1<=scycle && scycle<=256) {
            
        }
    } else if (241<=scanline && scanline<=260) {
        
    }

    // increment
    scycle++;
    scycle%=341;
    cycles++;
    if (scycle==0) {
        scanline++;
    }
    apply_and_update_registers();
}

void PPU::apply_and_update_registers() {
    if (scanline>=241 && scanline<=260) {
        *PPUSTATUS|=0x80;
    } else {
        *PPUSTATUS&=0x7F;
    }
}

void PPU::loadRom(ROM *r) {
    rom = r;
    uint8_t m = rom->get_mapper();
    switch(m) {
        case 0:
            memcpy(memory,rom->chr,rom->get_chrsize());
    }

}

