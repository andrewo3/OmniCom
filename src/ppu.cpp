#include "util.h"
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
    if (0<=scanline && scanline<=239) { // visible scanlines
        if (1<=scycle && scycle<=256) { //TODO: fetching background and sprite data during visible scanlines
            int intile = (scycle-1)%8; //get index into a tile (8 pixels in a tile)
            if (intile==0) { // beginning of a tile
                tile_addr = 0x2000 | (v & 0x0fff);
                attr_addr = 0x23c0 | (v & 0x0c00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07);
                uint8_t tile_val = read(&memory[tile_addr]);
                uint16_t pattern_table_loc = (((*PPUCTRL)&0x10)<<8)|((tile_val)<<4);
                ptlow=((uint8_t)memory[pattern_table_loc]); // add next low byte
                pthigh = (uint8_t)memory[pattern_table_loc|8]; // add next high byte
            } else if (intile==7) { // end of tile
                //increment v horizontally
                //pseudo code from: https://www.nesdev.org/wiki/PPU_scrolling#Wrapping_around
                if ((v&0x001F)==0x1F) { // if coarse X == 31, that means you reached the end of the nametable (next would be 32)
                    v &= ~0x001F; //un set coarse x to make it 0 again
                    v^= 0x0400; // switch nametable
                } else {
                    v++;
                }
            }

            //get pallete location and pixel color
            uint8_t attr_read = read(&memory[attr_addr]);
            bool right = tile_addr&1;
            bool bottom = (tile_addr>>1)&1;
            uint8_t attribute = (attr_read>>((right<<1)|(bottom<<2)))&3;
            uint8_t pattern = ((ptlow>>x)&1)|(((pthigh>>x)&1)<<1);
            uint8_t pixel = read(&memory[0x3f00+4*attribute+pattern]) ? pattern : read(&memory[0x3f00]);

            //write some pixel to image here
            int color_ind = pixel*3;

            for (int i=0; i<3; i++) {
                out_img[(scycle-1)+256*scanline+i] = NTSC_TO_RGB[color_ind+i];
            }
            x++;
            x%=8;
            

        }
        if (scycle==256) { // dot 256 of scanline
            //increment v vertically
            // pseudo code from: https://www.nesdev.org/wiki/PPU_scrolling#Wrapping_around
            if ((v & 0x7000) != 0x7000) { //if fine Y < 7
                v += 0x1000; // increment fine Y
            } else {
                v &= ~0x7000;
                int y = (v & 0x3e0) >> 5; // coarse y
                if (y==29) { // reset and switch nametable (29 is last row)
                    y = 0;
                    v ^= 0x8000;
                } else if (y==31) { // if 31, nametable doesnt switch
                    y = 0;
                } else {
                    y++;
                }
                v = (v & ~0x03e0)|(y<<5);
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

            //fourtable has nothing because four table is no mirroring at all
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

