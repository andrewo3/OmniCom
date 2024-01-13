#include "util.h"
#include "rom.h"
#include "ppu.h"
#include "cpu.h"
#include "mapper.h"
#include <cstring>
#include <cstdlib>
#include <mutex>

PPU::PPU() {
    this->scanline = 0;
    this->set_registers();
}

long long PPU::get_addr(int8_t* ptr) {
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
    if (rom->get_chrsize()==0 && get_addr(address)<0x2000) { //chr-ram
        rom->chr_ram[rom->mmc1chrbank*0x1000] = value;
    }
}

int8_t PPU::read(int8_t* address) {
    map_memory(&address);
    int8_t res;
    res = *address;
    if (rom->get_chrsize()==0 && get_addr(address)<0x2000) { //chr-ram
        rom->chr_ram[rom->mmc1chrbank*0x1000] = *address;
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
    bool rendering = ((*PPUMASK)&0x18); //checks if rendering is enabled
    if (0<=scanline && scanline<=239) { // visible scanlines
        int intile = (scycle-1)%8; //get index into a tile (8 pixels in a tile)
        if (1<=scycle && scycle<=256) {
            for (int i=0; i<scanlinespritenum; i++) {
                if (active_sprites&(1<<i)) { //if sprite already active
                    sprite_patterns[i]--; //subtract one from bit for pattern
                }
                if (!sprite_x_counters[i]) { //if x counter is 0, sprite becomes active
                    active_sprites|=(1<<i);
                } else if ((int8_t)sprite_x_counters[i]<-7) {
                    active_sprites&=~(1<<i);
                }
                sprite_x_counters[i]--;
            }
            //printf("Scanline %i Dot %i active sprites: %02x\n",scanline,scycle-1,active_sprites);
            if (intile==0 && rendering) { // beginning of a tile
                tile_addr = 0x2000 | (v & 0x0fff);
                attr_addr = 0x23c0 | (v & 0x0c00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07);
                uint8_t tile_val = read(&memory[tile_addr]);
                uint16_t pattern_table_loc = (((*PPUCTRL)&0x10)<<8)|((tile_val)<<4)|(((v&0x7000)>>12)&0x07);
                internalx = x&0x7;
                //internalx = 0;
                ptlow=(uint8_t)read(&memory[pattern_table_loc]); // add next low byte
                pthigh = (uint8_t)read(&memory[pattern_table_loc+8]); // add next high byte
            }

            if (scycle<=64 && ((*PPUMASK)&0x10)) { //secondary oam initialize
                for (int i=0; i<32; i++) {
                    secondary_oam[scycle/2]=0xff;
                    secondary_oam[scycle/2+1]=0xff;
                }
                sprites = 0;
                sprite_eval_n = 0;
                sprite_eval_m = 0;
                sprite_eval = true;
                sprite_eval_end = false;
                spritezeropresent = false;
            } else if (scycle<=256 && rendering) { //sprite evaluation
                if (!(scycle%2) && sprites<8) {
                    uint8_t sprite_y = oam[sprite_eval_n]+1;
                    if (!sprite_eval_end && sprite_y!=0) { // if you havent already reached the end of oam once
                        secondary_oam[sprites*4] = sprite_y; // copy y pos
                        bool h16 = (*PPUCTRL)&0x20;
                        if ((scanline+1-sprite_y)<(8<<h16) && (scanline+1-sprite_y)>=0 && sprite_y<240) {
                            memcpy(&secondary_oam[sprites*4+1],&oam[sprite_eval_n+1],3);
                            next_sprite_x_counters[sprites] = (uint8_t)secondary_oam[sprites*4+3];
                            if (sprite_eval_n==0) {
                                spritezeropresent = true;
                            }
                            sprites++;
                        }
                    }
                    sprite_eval_n+=4;
                    if (sprite_eval_n == 0) { // no more sprites in primary oam to evaluate for the next line
                        sprite_eval_end = true;
                    }
                } else if (sprites==8 && rendering) {
                    uint8_t sprite_y = oam[sprite_eval_n+sprite_eval_m];
                    bool h16 = (*PPUCTRL)&0x20;
                    if ((scanline-sprite_y)<(8<<h16) && (scanline-sprite_y)>=0) {
                        (*PPUSTATUS)|=0x20; //set sprite overflow
                        sprite_eval_n+=4;
                    } else {
                        sprite_eval_n+=4;

                        sprite_eval_m++; //the m increment is a hardware bug of the actual NES. for expected behavior of sprite overflow flag it actually shouldnt do this.
                        sprite_eval_m%=4;
                    }
                }
            }

            if (scycle==256 && rendering) { // end of scanline, increment vertically and wrap around
                v_vert();
            }

                //printf("%04x, %04x - %i, %i, %04x\n",tile_addr, attr_addr, scycle, scanline, v);
            
            //RENDERING

            //get background pallete location and pixel color
            uint8_t attr_read = read(&memory[attr_addr]);
            bool right = (tile_addr>>1)&1;
            bool bottom = (tile_addr>>6)&1;
            uint8_t attribute = (attr_read>>((right<<1)|(bottom<<2)))&3;
            uint8_t flip = 7-internalx;
            uint8_t bg_pattern = ((ptlow>>flip)&1)|(((pthigh>>flip)&1)<<1);
            
            
            //get sprite information to multiplex over background
            uint8_t sprite_pattern = 0;
            uint8_t sprite_palette = 0;
            uint8_t sprite_index = 0;
            bool sprite_priority = 1;
            uint8_t sprite_y = 0;
            for (int i=scanlinespritenum-1; i>=0; i--) { // go in reverse to get lower priority first. This will result in highest priority pixels on top.
                
                if (active_sprites&(1<<i)) { //if sprite is active
                    //draw sprite
                    uint8_t sprite_bit = sprite_patterns[i];
                    if (sprite_bit>=0) {
                        sprite_y=scanlinesprites[4*i];
                        uint8_t sprite_tile_ind = scanlinesprites[4*i+1]&0xff;
                        bool sprite_bank = (*PPUCTRL)&0x8;
                        bool h16 = (*PPUCTRL)&0x20;
                        if (h16) {
                            sprite_tile_ind = sprite_tile_ind&0xfe;
                            sprite_bank = sprite_tile_ind&0x1;
                        }
                        uint8_t sprite_attr = scanlinesprites[4*i+2];
                        uint8_t new_sprite_palette = sprite_attr&0x3;
                        bool flip_x = sprite_attr&0x40;
                        bool flip_y = sprite_attr&0x80;
                        uint8_t local_y = flip_y ? 7+8*h16-(scanline-sprite_y) : (scanline-sprite_y);
                        sprite_tile_ind+=local_y/8;
                        uint16_t sprite_tile;
                        sprite_tile = (sprite_bank<<12)|((sprite_tile_ind<<4)+(local_y>7))|(local_y&0x7);
                        if (flip_x) {
                            sprite_bit = 7-sprite_bit;
                        }
                        uint8_t sprite_x = scanlinesprites[4*i+3]&0xff;
                        
                        uint8_t new_sprite_pattern = ((read(&memory[sprite_tile])>>sprite_bit)&1)|(((read(&memory[sprite_tile|8])>>sprite_bit)&1)<<1);
                        if (new_sprite_pattern!=0) {
                            sprite_pattern = new_sprite_pattern;
                            sprite_palette = new_sprite_palette;
                            sprite_index = i;
                            sprite_priority = sprite_attr&0x20;
                        }
                    }
                }
            }
            uint8_t pattern;
            bool sprite_pix = false;
            if (bg_pattern) {
                if (sprite_pattern) {
                    switch(sprite_priority) {
                        case 0:
                            sprite_pix = true;
                            break;
                    }
                }
            } else {
                if (sprite_pattern) {
                    sprite_pix = true;
                }
            }
            bool sprite0hit = false;
            if (nextspritezeropresent && sprite_index==0 && ((*PPUMASK)&0x8) && ((*PPUMASK)&0x10) && !(sprite_pattern == 0) && !(bg_pattern == 0) && !((*PPUSTATUS)&0x40)) { //if sprite zero is in the secondary oam, and sprite index was the first one (which must have been sprite 0), this is a sprite 0 hit
                //sprite 0 hit
                (*PPUSTATUS)|=0x40;
                sprite0hit = true;
            }
            pattern = (sprite_pix &&((*PPUMASK)&0x10))  ? sprite_pattern : bg_pattern;
            if (sprite_pix) {
                attribute = sprite_palette;
            } else {
                if (!((*PPUMASK)&0x8)) {
                    pattern = 0;
                }
            }
            uint8_t pixel = pattern ? read(&memory[(0x3f00|(0x10*sprite_pix))+4*attribute+pattern]) : read(&memory[(0x3f00|(0x10*sprite_pix))]);
            //printf("POS(%i,%i) - TILEIND $%04x: %02x, ATTRIBUTE: %04x, PATTERN - $%04x: %02x %02x,bit: %i, val: %i, finey: %i\n",scycle-1,scanline,tile_addr,read(&memory[tile_addr]),attr_addr,(((*PPUCTRL)&0x10)<<8)|((read(&memory[tile_addr]))<<4)|(((v&0x7000)>>12)&0x07),ptlow,pthigh, internalx, pattern,(((v&7000)>>12)&0x07));
            //write some pixel to image here
            int color_ind = pixel*3;

            for (int i=0; i<3; i++) {
                out_img[3*((scycle-1)+256*scanline)+i] = NTSC_TO_RGB[color_ind+i];
            }
            if ((*PPUMASK)&0x80) {
                out_img[3*((scycle-1)+256*scanline)+2] = 255;
            }
            if ((*PPUMASK)&0x40) {
                out_img[3*((scycle-1)+256*scanline)+1] = 255;
            }
            if ((*PPUMASK)&0x20) {
                out_img[3*((scycle-1)+256*scanline)] = 255;
            }
            
            internalx++;
            if (internalx==8) {
                internalx%=8;
                uint16_t fake_v = v;
                //increment v horizontally
                //pseudo code from: https://www.nesdev.org/wiki/PPU_scrolling#Wrapping_around
                if ((fake_v&0x001F)==0x1F) { // if coarse X == 31, that means you reached the end of the nametable row (next would be 32)
                    fake_v &= ~0x001F; //un set coarse x to make it 0 again
                    fake_v^= 0x0400; // switch nametable
                } else {
                    fake_v++;
                }
                tile_addr = 0x2000 | (fake_v & 0x0fff);
                attr_addr = 0x23c0 | (fake_v & 0x0c00) | ((fake_v >> 4) & 0x38) | ((fake_v >> 2) & 0x07);
                uint8_t tile_val = read(&memory[tile_addr]);
                uint16_t pattern_table_loc = (((*PPUCTRL)&0x10)<<8)|((tile_val)<<4)|(((v&0x7000)>>12)&0x07);
                //internalx = 0;
                ptlow=(uint8_t)read(&memory[pattern_table_loc]); // add next low byte
                pthigh = (uint8_t)read(&memory[pattern_table_loc+8]); // add next high byte
            }

        } else if (scycle == 257 && rendering) {
            v&=~0x41F;
            v|=(t&0x41F);
            oam_addr = 0;
            memcpy(scanlinesprites,secondary_oam,sprites*4); //copy sprites
            memcpy(sprite_x_counters,next_sprite_x_counters,8);
            nextspritezeropresent = spritezeropresent;
            scanlinespritenum = sprites;
            active_sprites = 0;
            for (int i=0; i<sprites; i++) {
                sprite_patterns[i]=7; //set bit to 7 for each sprite pattern
            }
        }
        if (intile==7 && rendering && (scycle>=337 || scycle<256)) {
            v_horiz();
        }
    } else if (241<=scanline && scanline<=260) { //vblank
        //printf("vblank!\n");
        if (vblank==false && scycle>=1) { //start vblank as soon as you reach this
            vblank = true;
            image_mutex.unlock();
            *PPUSTATUS|=0x80;
            if ((*PPUCTRL)&0x80) { // if ppu is configured to generate nmi, do so.
                cpu->recv_nmi = true;
                //printf("NMI\n");
            }

        }
    } else if (scanline==261) { // pre-render scanline
        if (scycle==1) {
            (*PPUSTATUS)&=~0xE0; //clear overflow, sprite 0 hit, and vbl
        }
        if (scycle>=280 && scycle<=304 && rendering) {
            v &= ~0x7BE0;
            v |= (t&0x7BE0);
        }
        if (scycle==1 && vblank==true) {
            *PPUSTATUS&=~0x80;
            vblank = false;
            image_mutex.lock();
        }

    }
    if (*PPUSTATUS&0x80) {
        vbl_count++;
    } else if (vbl_count!=0) {
        //printf("VBL PPU Clocks: %i\n",vbl_count);
        vbl_count = 0;
    }

    // increment
    scycle++;
    scycle%=341;
    cycles++;
    if (scycle==0) {
        scanline++;
        scanline%=262;
        if (frames%2==1 && scanline==0 && rendering) { // skip odd frames when rendering
            cycles++;
            scycle++;
        }
        frames++;
    }
    apply_and_update_registers();
}

void PPU::apply_and_update_registers() {
    if (!(scanline>=241 && scanline<=260)) {
        *PPUSTATUS&=0x7F;
    }
}

void PPU::map_memory(int8_t** addr) {
    long long location = get_addr(*addr);
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
    } else if (location==0x3f10 || location==0x3f14 || location==0x3f18 || location==0x3f1c) {
        *addr-=0x10;
    } else if (0x3F20<=location&&location<0x4000) {
        *addr-=(location-0x3f00)/0x20*0x20;
    }
}

void PPU::loadRom(ROM *r) {
    rom = r;
    memcpy(memory,rom->get_chr_bank(chr_bank_num),0x2000);

}

