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

void PPU::write(int16_t address, int8_t value) { //write ppu memory, taking into account any mirrors or bankswitches
    map_memory(address);
    memory[address] = value;
}

int8_t PPU::read(int16_t address) {
    map_memory(address);
    int8_t res;
    res = memory[address];
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
    if (scanline>=0 && scanline<=239) { // visible scanlines
        if (image_drawn && !mutex_locked) {
            if (image_mutex.try_lock()) {
                mutex_locked = true;
            }
        }
        int scan_cyc = scycle-1;
        int intile = scan_cyc%8; //get index into a tile (8 pixels in a tile)
        if (scycle==0 && rendering) {
            address_bus = (((*PPUCTRL)&0x10)<<8)|((read(0x2000 | (v & 0x0fff)))<<4)|(((v&0x7000)>>12)&0x07);
        }

        //address bus variable set (used by mappers)
        if (((0<=scan_cyc && scan_cyc<256)||(320<=scan_cyc && scan_cyc<340)) && ((*PPUMASK)&0x8)) { //cycles 1-320 address bus fetches
            switch(intile/2) {
                case 0:
                    address_bus = 0x2000 | (v & 0x0fff); //nt
                    break;
                case 1:
                    address_bus = 0x23c0 | (v & 0x0c00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07); //attr_table
                    break;
                case 2:
                    address_bus = (((*PPUCTRL)&0x10)<<8)|((read(0x2000 | (v & 0x0fff))&0xff)<<4)|(((v&0x7000)>>12)&0x07); //pt low
                    break;
                case 3:
                    address_bus += 8; //add 8 for upper pattern table
                    break;
            }
        } else if (256<=scan_cyc && scan_cyc<320 && ((*PPUMASK)&0x10)) { //fetch sprite on address bus
            bool sprite16 = (*PPUCTRL)&0x20;
            switch(((scan_cyc-256)%8)/2) {
                case 0:
                    address_bus = 0x2000 | (v & 0x0fff); //nt
                    break;
                case 1:
                    address_bus = 0x23c0 | (v & 0x0c00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07); //attr_table
                    break;
                case 2:
                    {
                    int s = ((scan_cyc-256)/8)*4+1;
                    address_bus = sprite16 ? 
                    (((*PPUCTRL)&0x8)<<9)|((secondary_oam[s]&0xff)<<4) : 
                    ((secondary_oam[s]&0x1)<<12)|((secondary_oam[s]&0xfe)<<4);
                    break;
                    }
                case 3:
                    address_bus += 8; //add 8 for upper pattern table
                    break;
            }
        }
        if (0<=scan_cyc && scan_cyc<256) {
            for (int i=0; i<scanlinespritenum; i++) {
                if (active_sprites&(1<<i)) { //if sprite already active
                    sprite_patterns[i]--; //subtract one from bit for pattern
                }
                if (!sprite_x_counters[i] && scan_cyc>=0) { //if x counter is 0, sprite becomes active
                    active_sprites|=(1<<i);
                } else if (sprite_x_counters[i]<-7) {
                    active_sprites&=~(1<<i);
                }
                sprite_x_counters[i]--;
            }
            //printf("Scanline %i Dot %i active sprites: %02x\n",scanline,scycle-1,active_sprites);
            if (intile==0 && rendering) { // beginning of a tile
                tile_addr = 0x2000 | (v & 0x0fff);
                attr_addr = 0x23c0 | (v & 0x0c00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07);
                tile_val = read(tile_addr);
                pattern_table_loc = (((*PPUCTRL)&0x10)<<8)|((tile_val)<<4)|(((v&0x7000)>>12)&0x07);
                internalx = x&0x7;
                //internalx = 0;
                ptlow=(uint8_t)read(pattern_table_loc); // add next low byte
                pthigh = (uint8_t)read(pattern_table_loc+8); // add next high byte
            }

            if (scycle==1 && ((*PPUMASK)&0x10)) { //secondary oam initialize
                for (int i=0; i<32; i++) {
                    secondary_oam[i]=0xff;
                    //secondary_oam[scycle/2+1]=0xff;
                }
                sprites = 0;
                sprite_eval_n = 0;
                sprite_eval_m = 0;
                sprite_eval = true;
                sprite_eval_end = false;
                spritezeropresent = false;
            } else if (scycle>128 && scycle<=256 && rendering) { //sprite evaluation
                if (!(scycle%2) && sprites<8) {
                    uint8_t sprite_y = oam[sprite_eval_n]+1;
                    if (!sprite_eval_end && sprite_y!=0) { // if you havent already reached the end of oam once
                        secondary_oam[sprites*4] = sprite_y; // copy y pos
                        bool h16 = (*PPUCTRL)&0x20;
                        if (((scanline+1)%240-sprite_y)<(8<<h16) && ((scanline+1)%240-sprite_y)>=0 && sprite_y<240) {
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
            uint8_t attr_read = read(attr_addr);
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
                            sprite_bank = (sprite_tile_ind&0x1);
                            sprite_tile_ind = sprite_tile_ind&0xfe;
                            
                        }
                        uint8_t sprite_attr = scanlinesprites[4*i+2];
                        uint8_t new_sprite_palette = sprite_attr&0x3;
                        bool flip_x = sprite_attr&0x40;
                        bool flip_y = sprite_attr&0x80;
                        uint8_t local_y = flip_y ? 7+8*h16-(scanline-sprite_y) : (scanline-sprite_y);
                        sprite_tile_ind+=local_y/8;
                        uint16_t sprite_tile;
                        sprite_tile = (sprite_bank<<12)|((sprite_tile_ind<<4))|(local_y&0x7);
                        if (flip_x) {
                            sprite_bit = 7-sprite_bit;
                        }
                        uint8_t sprite_x = scanlinesprites[4*i+3]&0xff;
                        
                        uint8_t new_sprite_pattern = ((read(sprite_tile)>>sprite_bit)&1)|(((read(sprite_tile|8)>>sprite_bit)&1)<<1);
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
            //if not drawing on leftmost 8 pixels
            if (scycle<=8 && !((*PPUMASK)&0x2) && !sprite_pix) {
                pattern = 0;
            } else if (scycle<=8 && !((*PPUMASK)&0x4) && sprite_pix) {
                pattern = 0;
                sprite_pix = 0;
            }
            uint8_t pixel = pattern ? read((0x3f00|(0x10*sprite_pix))+4*attribute+pattern) : read((0x3f00|(0x10*sprite_pix)));
            //printf("POS(%i,%i) - TILEIND $%04x: %02x, ATTRIBUTE: %04x, PATTERN - $%04x: %02x %02x,bit: %i, val: %i, finey: %i\n",scycle-1,scanline,tile_addr,read(&memory[tile_addr]),attr_addr,(((*PPUCTRL)&0x10)<<8)|((read(&memory[tile_addr]))<<4)|(((v&0x7000)>>12)&0x07),ptlow,pthigh, internalx, pattern,(((v&7000)>>12)&0x07));
            //write some pixel to image here
            int color_ind = pixel*3;
            int pix_loc = 3*(scan_cyc+(scanline<<8));
            internal_img[pix_loc] = NTSC_TO_RGB[color_ind];
            internal_img[pix_loc+1] = NTSC_TO_RGB[color_ind+1];
            internal_img[pix_loc+2] = NTSC_TO_RGB[color_ind+2];
            if ((*PPUMASK)&0x80) {
                internal_img[pix_loc+2] /= 2;
                internal_img[pix_loc+2] += 127;
            }
            if ((*PPUMASK)&0x40) {
                internal_img[pix_loc+1] /= 2;
                internal_img[pix_loc+1] += 127;
            }
            if ((*PPUMASK)&0x20) {
                internal_img[pix_loc] /= 2;
                internal_img[pix_loc] += 127;
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
                uint8_t tile_val = read(tile_addr);
                uint16_t pattern_table_loc = (((*PPUCTRL)&0x10)<<8)|((tile_val)<<4)|(((v&0x7000)>>12)&0x07);
                //internalx = 0;
                ptlow=(uint8_t)read(pattern_table_loc); // add next low byte
                pthigh = (uint8_t)read(pattern_table_loc+8); // add next high byte
            }

        } else if (scycle == 257 && rendering) {
            v&=~0x41F;
            v|=(t&0x41F);
            oam_addr = 0;
            memcpy(scanlinesprites,secondary_oam,sprites*4); //copy sprites
            memcpy(sprite_x_counters,next_sprite_x_counters,8*sizeof(int));
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
        if (vblank==false && scycle==1 && scanline==241) { //start vblank as soon as you reach this
            vblank = true;
            memcpy(current_img,internal_img,sizeof(uint8_t)*184320); //copy internal img to out image every frame update
            image_mutex.unlock();
            mutex_locked = false;
            image_drawn = false;
            if (!nmi_suppress) {
                nmi_occurred = true;
            }
            if (!disable_vbl) {
                *PPUSTATUS|=0x80;
            }
        }
    } else if (scanline==261) { // pre-render scanline
        if (scycle==1) {
            nmi_suppress = false;
            disable_vbl = false;
            nmi_occurred = false;
            (*PPUSTATUS)&=~0xE0; //clear overflow, sprite 0 hit, and vbl
        }
        if (scycle>=280 && scycle<=304 && rendering) {
            v &= ~0x7BE0;
            v |= (t&0x7BE0);
        }
        if (scycle==1 && vblank==true) {
            (*PPUSTATUS)&=~0x80;
            vblank = false;
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
    if (scycle==339 && frames%2==1 && scanline==261 && ((*PPUMASK)&0x8))
        scycle++;
    scycle%=341;
    cycles++;
    if (scycle==0) {
        scanline++;
        scanline%=262;
        if (scanline==0) {
            frames++;
        }
    }
    mapper->clock(&system[0]);
    //apply_and_update_registers();
}

void PPU::apply_and_update_registers() {
    if (!(scanline>=241 && scanline<=260)) {
        *PPUSTATUS&=0x7F;
    }
}

void PPU::map_memory(int16_t &location) {
    if ((location & 0xf000) == 0x2000) { //map according to rom, which could also include CHR bankswitching
        switch(mirrormode) {
            case HORIZONTAL:
                location -= location&0x400; //horizontal nametable mirroring
                break;
            case VERTICAL:
                location -= location&0x800; //vertical nametable mirroring
                break;
            case SINGLESCREEN:
                location = 0x2000|(location&0x3ff);
                break;

            //fourtable has nothing because four table is no mirroring at all
        }
    }
    else if (0x3000 <= location && location < 0x3f00) {
        location-=0x1000;
    } else if ((location&(~0xc))==0x3f10) {
        location&=~0xf0;
    } else if ((location & 0xff00)==0x3f00) {
        location&=~0xe0;
    }
}

void PPU::loadRom(ROM *r) {
    rom = r;
    mirrormode = rom->mirrormode;
    system[0] = cpu;
    system[1] = this;
    system[2] = cpu->apu;
    mapper = rom->get_mapper();
    //printf("PPU CHR SIZE: %i\n",rom->get_chrsize());
    if (rom->get_chrsize()>0) {
        memcpy(memory,rom->get_chr_bank(chr_bank_num),0x2000);
    } else {
        printf("CHR-RAM Copied.\n");
        rom->chr_ram = (uint8_t*)&memory[0];
    }

}

