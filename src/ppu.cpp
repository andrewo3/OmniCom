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
    if (rom->get_chrsize()==0 && get_addr(address)<0x2000) { //chr-ram
        chr_ram[get_addr(address)] = value;
    } else {
        *address = value;
    }
}

int8_t PPU::read(int8_t* address) {
    map_memory(&address);
    int8_t res;
    if (rom->get_chrsize()==0 && get_addr(address)<0x2000) { //chr-ram
        res = chr_ram[get_addr(address)];
    } else {
        res = *address;
    }
    return res;
}

void PPU::v_horiz() {
    //increment v horizontally
    //pseudo code from: https://www.nesdev.org/wiki/PPU_scrolling#Wrapping_around
    if ((v&0x001F)==0x1F) { // if coarse X == 31, that means you reached the end of the nametable row (next would be 32)
        v &= ~0x001F; //un set coarse x to make it 0 again
        v^= 0x0400; // switch nametable
    } else {
        v++;
    }
}

void PPU::v_vert() {
    //increment v vertically
    // pseudo code from: https://www.nesdev.org/wiki/PPU_scrolling#Wrapping_around
    if ((v & 0x7000) != 0x7000) { //if fine Y < 7
        v += 0x1000; // increment fine Y
    } else {
        v &= ~0x7000;
        int y = (v & 0x3e0) >> 5; // coarse y
        if (y==29) { // reset and switch nametable (29 is last row)
            y = 0;
            v ^= 0x0800;
        } else if (y==31) { // if 31, nametable doesnt switch
            y = 0;
        } else {
            y++;
        }
        v = (v & ~0x03e0)|(y<<5);
    }
}

void PPU::cycle() {
    bool rendering = !((~(*PPUMASK))&0xC); //checks if rendering is enabled
    if (0<=scanline && scanline<=239) { // visible scanlines
        int intile = (scycle-1)%8; //get index into a tile (8 pixels in a tile)
        if (1<=scycle && scycle<=256 && rendering) { //TODO: fetching background and sprite data during visible scanlines
            if (intile==0) { // beginning of a tile
                tile_addr = 0x2000 | (v & 0x0fff);
                attr_addr = 0x23c0 | (v & 0x0c00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07);
                uint8_t tile_val = read(&memory[tile_addr]);
                uint16_t pattern_table_loc = (((*PPUCTRL)&0x10)<<8)|((tile_val)<<4)|(((v&0x7000)>>12)&0x07);
                internalx = x;
                ptlow=(uint8_t)read(&memory[pattern_table_loc]); // add next low byte
                pthigh = (uint8_t)read(&memory[pattern_table_loc+8]); // add next high byte
            } if (intile==7) { // end of tile
                if (scycle==256) { // dot 256 of scanline
                    v_vert();
                } else {
                    v_horiz();
                }
                //printf("%04x, %04x - %i, %i, %04x\n",tile_addr, attr_addr, scycle, scanline, v);
            }
            //get pallete location and pixel color
            uint8_t attr_read = read(&memory[attr_addr]);
            bool right = (tile_addr>>1)&1;
            bool bottom = (tile_addr>>6)&1;
            //11011101;
            uint8_t attribute = (attr_read>>((right<<1)|(bottom<<2)))&3;
            uint8_t flip = 7-internalx;
            uint8_t pattern = ((ptlow>>flip)&1)|(((pthigh>>flip)&1)<<1);
            uint8_t pixel = pattern ? read(&memory[0x3f00+4*attribute+pattern]) : read(&memory[0x3f00]);
            //printf("POS(%i,%i) - TILEIND $%04x: %02x, ATTRIBUTE: %04x, PATTERN - $%04x: %02x %02x,bit: %i, val: %i, finey: %i\n",scycle-1,scanline,tile_addr,read(&memory[tile_addr]),attr_addr,(((*PPUCTRL)&0x10)<<8)|((read(&memory[tile_addr]))<<4)|(((v&0x7000)>>12)&0x07),ptlow,pthigh, internalx, pattern,(((v&7000)>>12)&0x07));
            //write some pixel to image here
            int color_ind = pixel*3;

            for (int i=0; i<3; i++) {
                out_img[3*((scycle-1)+256*scanline)+i] = NTSC_TO_RGB[color_ind+i];
            }
            internalx++;
            internalx%=8;

        } else if (scycle>=328 && intile==7 && rendering) { // after the first few dots
            v_horiz(); 
        }   else if (scycle == 257 && rendering) {
            v&=~0x41F;
            v|=(t&0x41F);
        }
    } else if (241<=scanline && scanline<=260) { //vblank
        if (vblank==false) { //start vblank as soon as you reach this
            vblank = true;
            *PPUSTATUS|=0x80;
            if ((*PPUCTRL)&0x80) { // if ppu is configured to generate nmi, do so.
                cpu->recv_nmi = true;
                //printf("NMI\n");
            }

        }
    } else if (scanline==261) { // pre-render scanline
        if (((*PPUMASK)&0xC) && scycle>=280 && scycle<=304) {
            v &= ~0x7BE0;
            v |= (t&0x7BE0);
        }
        if (scycle == 340 && vblank == true) {
            if (((*PPUMASK)&0xC)) {
                //v &= 0x841F;
                //v |= (t&0x7BE0);
                t = v;
                //v = 0x2000+(0x400*(*PPUCTRL&0x3));
            }
            //t = v;
            *PPUSTATUS&=0x7F;
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
                break;
            case VERTICAL:
                *addr -= location>=0x2800 ? 0x800 : 0; //horizontal nametable mirroring
                break;

            //fourtable has nothing because four table is no mirroring at all
        }
    }
    else if (0x3000<=location && location<0x3F00) {
        *addr-=0x1000;
    } else if (0x3F20<=location&&location<0x4000) {
        *addr-=(location-0x3f00)/0x20*0x20;
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

