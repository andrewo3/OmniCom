#include "rom.h"
#include "ppu.h"
#include "cpu.h"
#include <cstring>

PPU::PPU() {
    this->scanline = 0;
    this->set_registers();
}

uint16_t PPU::get_addr(int8_t* ptr) {
    return ptr-memory;
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

void PPU::write(int8_t* address, int8_t value) { //write ppu memory, taking into account any mirrors or bankswitches
    map_memory(&address);
    *address = value;
}

int8_t PPU::read(int8_t* address) {
    map_memory(&address);
    return *address;
}

void PPU::cycle() {
    int8_t* vram_ptr = &memory[vram_addr];
    if (0<=scanline && scanline<=239) { // visible scanlines
        vblank = false;
        if (1<=scycle && scycle<=256) {
            int intile = (scycle-1)%8;
            if (intile==0) {
                int8_t r = read(vram_ptr);
            }
        }
    } else if (241<=scanline && scanline<=260) { //vblank
        if (vblank==false) { //start vblank as soon as you reach this
            vblank = true;
            *PPUSTATUS|=0x80;
            //TODO: push image to variable, so that SDL+OpenGL can take over and draw it to window
            if ((*PPUCTRL)&0x80) { // if ppu is configured to generate nmi, do so.
                cpu->recv_nmi = true;
                //printf("NMI\n");
            }

        }
    } else if (scanline==261) { // pre-render scanline
        if (vblank == true) {
            vblank = false;
        }


    }

    // increment
    scycle++;
    scycle%=341;
    cycles++;
    if (scycle==0) {
        scanline++;
        scanline%=262;
    }
    apply_and_update_registers();
}

void PPU::apply_and_update_registers() {
    if (!(scanline>=241 && scanline<=260)) {
        *PPUSTATUS&=0x7F;
    }
}

void PPU::map_memory(int8_t** addr) {
    uint16_t location = get_addr(*addr);
    if (0x2000<=location && location<0x3000) { //map according to rom, which could also include CHR bankswitching
        switch(rom->mirrormode) {
            case HORIZONTAL:
                *addr -= ((location-0x2000)/0x400)%2 ? 0x400 : 0; //horizontal nametable mirroring
            case VERTICAL:
                *addr -= location>=0x2800 ? 0x800 : 0; //horizontal nametable mirroring
        }
    }
    else if (0x3000<=location && location<0x4000) {
        *addr-=0x1000;
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

