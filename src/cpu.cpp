#include "rom.h"
#include "cpu.h"
#include "ppu.h"
#include "util.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <SDL2/SDL.h>

// description of addressing modes:
// https://blogs.oregonstate.edu/ericmorgan/2022/01/21/6502-addressing-modes/
//Addressing modes more in depth: https://wiki.cdot.senecacollege.ca/wiki/6502_Addressing_Modes
//Flags: https://www.nesdev.org/wiki/Status_flags
//Memory Map: https://www.nesdev.org/wiki/CPU_memory_map
//Opcode Table: https://www.masswerk.at/6502/6502_instruction_set.html#layout
//Instruction Descriptions: https://www.nesdev.org/obelisk-6502-guide/reference.html
//program execution steps: https://www.middle-engine.com/blog/posts/2020/06/23/programming-the-nes-the-6502-in-detail

CPU::CPU() {
    define_opcodes();
}

CPU::CPU(bool dbug) {
    this->debug = dbug;
    define_opcodes();
}

void CPU::start_nmi() {
    //printf("NMI\n");
    recv_nmi = false;
    uint16_t push = get_addr(pc);
    stack_push((uint8_t)(push>>8));
    stack_push((uint8_t)(push&0xff));
    stack_push(flags);
    set_flag('I',true);

    int8_t * res = &memory[NMI];
    pc = abs(res); 
}

void CPU::write(int8_t* address, int8_t value) {
    map_memory(&address);
    long long mem = get_addr(address); 

    if (debug) {
        printf("%04x=>%02x\n",mem,value&0xff);
    }
    switch(mem) {
        //write to OAMADDR (0x2001) is handled implicitly
        //write to OAMADDR (0x2003) is handled implicitly
        case 0x2003:
            ppu->oam_addr = (uint8_t)value;
            break;
        case 0x2000:
            //printf("(Before) Write %02x->0x%04x: v=%04x,t=%04x,w=%i,x=%02x\n",value&0xff,mem,ppu->v,ppu->t,ppu->w,ppu->x);
            ppu->t &= ~0xC00;
            ppu->t |= (value&0x3)<<10;
            //printf("(After) Write %02x->0x%04x: v=%04x,t=%04x,w=%i,x=%02x\n",value&0xff,mem,ppu->v,ppu->t,ppu->w,ppu->x);
            break;
        case 0x2004: //write to OAMDATA
            if (ppu->oam_addr%4==2) {
                value &= ~0x1C;
            }
            ppu->oam[ppu->oam_addr] = value;
            ppu->oam_addr++;
            break;
        case 0x2005: //write to PPUSCROLL
            //printf("(Before) Write %02x->0x%04x: v=%04x,t=%04x,w=%i,x=%02x\n",value&0xff,mem,ppu->v,ppu->t,ppu->w,ppu->x);
            if (!ppu->w) {
                ppu->t &= ~0x1F; //mask for bit replacement
                ppu->t |= (((uint8_t)value)>>3)&0x1F;
                ppu->x = value&0x7;
                ppu->w = 1;
            } else {
                ppu->t &= ~0x73e0; //another bit mask for replace bits;
                ppu->t |= (value&0xf8)<<2;
                ppu->t |= (value&0x7)<<12;
                ppu->w = 0;
            }
            //printf("(After) Write %02x->0x%04x: v=%04x,t=%04x,w=%i,x=%02x\n",value&0xff,mem,ppu->v,ppu->t,ppu->w,ppu->x);
            break;
        case 0x2006: //write to PPUADDR
            //printf("(Before) Write %02x->0x%04x: v=%04x,t=%04x,w=%i,x=%02x\n",value&0xff,mem,ppu->v,ppu->t,ppu->w,ppu->x);
            if (!ppu->w) {
                ppu->t &= ~0x3F00;
                ppu->t |= (value&0x3f)<<8;
                ppu->t &= ~0x4000;
                ppu->w = 1;
            } else {
                ppu->t &= ~0xff;
                ppu->t |= value&0xff;
                ppu->v = ppu->t;
                ppu->w = 0;
            }
            //printf("(After) Write %02x->0x%04x: v=%04x,t=%04x,w=%i,x=%02x\n",value&0xff,mem,ppu->v,ppu->t,ppu->w,ppu->x);
            break;
        case 0x2007: // write to PPUDATA
            {
            //printf("(Before) Write %02x->0x%04x: v=%04x,t=%04x,w=%i,x=%02x\n",value&0xff,mem,ppu->v,ppu->t,ppu->w,ppu->x);
            uint16_t bit14 = (ppu->v)&0x3fff;
            //printf("ppu->%04x: %02x\n",bit14,(uint8_t)value);
            ppu->write(&(ppu->memory[bit14]),value); // write method takes mapper into account
            ppu->v+=(memory[0x2000]&0x04) ? 0x20 : 0x01;
            //ppu->v&=~0x3fff;
            //ppu->v|=bit14;
            //printf("(After) Write %02x->0x%04x: v=%04x,t=%04x,w=%i,x=%02x\n",value&0xff,mem,ppu->v,ppu->t,ppu->w,ppu->x);
            break;
            }
        case 0x4014: //write to OAMDMA
            memcpy(ppu->oam,&memory[(uint16_t)value<<8],256);
            break;
        case 0x4016: //controller input 1
            if (value==1) {
                //poll input
                inputs = state[SDL_SCANCODE_RIGHT]| //right
                (state[SDL_SCANCODE_LEFT]<<1)| //left
                (state[SDL_SCANCODE_DOWN]<<2)| //down
                (state[SDL_SCANCODE_UP]<<3)| //up
                (state[SDL_SCANCODE_RETURN]<<4)| //start
                (state[SDL_SCANCODE_TAB]<<5)| //select
                (state[SDL_SCANCODE_LSHIFT]<<6)| //B
                (state[SDL_SCANCODE_SPACE]<<7); //A
            }
            break;
    }
    // special mapper cases
    switch (rom->get_mapper()) {
        case 1:
            {
            if (0x8000<=mem && mem<=0xFFFF) { //MMC1 shift register
                if (value&0x80) {
                    rom->mmc1shift = 0x10;
                    memcpy(&memory[0xC000],rom->get_prg_bank((rom->get_prgsize()/0x4000)-1),0x4000);
                } else {
                    bool full = rom->mmc1shift&1; //1 in bit 0
                    rom->mmc1shift>>=1;
                    rom->mmc1shift|=(value&1)<<4;
                    printf("SHIFT: %02x\n",rom->mmc1shift);
                    if (full) {
                        bool eightkb = rom->mmc1bankmode&0x10;
                        uint8_t prgmode = (rom->mmc1bankmode&0xC)>>2;
                        printf("MMC1 Stuff - %04x: %02x\n",mem,rom->mmc1shift);
                        if (0x8000<=mem && mem<=0x9FFF) { //MMC1 control
                            rom->mmc1bankmode = rom->mmc1shift;
                            eightkb = rom->mmc1bankmode&0x10;
                            prgmode = (rom->mmc1bankmode&0xC)>>2;
                            printf("PRGMODE: %i\n",prgmode);
                            uint8_t mirror = rom->mmc1shift&0x3;
                            if (mirror==2) {
                                rom->mirrormode = VERTICAL;
                            } else if (mirror ==3) {
                                rom->mirrormode = HORIZONTAL;
                            } else {
                                //other thing - one-screen - everything mirrors
                            }
                            if (prgmode==2) {
                                memcpy(&memory[0x8000],rom->get_prg_bank(0),0x4000);
                            } else if (prgmode==3) {
                                memcpy(&memory[0xC000],rom->get_prg_bank((rom->get_prgsize()/0x4000)-1),0x4000);
                            } else {
                                memcpy(&memory[0x8000],rom->get_prg_bank(rom->mmc1prgloc),0x8000);
                            }
                        } else if (0xA000<=mem && mem<=0xBFFF) { //CHR bank 0
                            uint8_t mask = 0x1e|eightkb; // test for 8kb mode in chr
                            rom->mmc1chrloc0 = rom->mmc1shift&mask;
                            rom->mmc1chrbank = rom->mmc1shift;
                            if (rom->get_chrsize()==0) { //chr-ram
                                rom->mmc1chrbank = rom->mmc1shift;
                                memcpy(ppu->memory,rom->get_chr_bank(rom->mmc1shift&mask),0x1000<<(!eightkb));
                            } else {
                                printf("NOT CHR-RAM\n");
                                rom->mmc1chrbank = rom->mmc1shift;
                                memcpy(ppu->memory,rom->get_chr_bank(rom->mmc1shift&mask),0x1000<<(!eightkb));
                                throw(0); //TODO
                            }
                        } else if (0xC000<=mem && mem<=0xDFFF && !eightkb) { //CHR bank 1
                            if (rom->get_chrsize()==0) { //chr-ram
                                rom->mmc1chrbank = rom->mmc1shift;
                                rom->mmc1chrloc1 = 1;
                                memcpy(&(ppu->memory[0x1000]),rom->get_chr_bank(rom->mmc1shift),0x1000);
                            } else {
                                printf("NOT CHR-RAM\n");
                                rom->mmc1chrbank = rom->mmc1shift;
                                rom->mmc1chrloc1 = rom->mmc1shift;
                                memcpy(&(ppu->memory[0x1000]),rom->get_chr_bank(rom->mmc1shift),0x1000);
                                throw(0); //TODO
                            }
                        } else if (0xE000<=mem && mem<=0xFFFF) { //PRG bank
                            rom->mmc1prgloc = rom->mmc1shift &0xf;
                            if (prgmode==2) {
                                memcpy(&memory[0x8000],rom->get_prg_bank(0),0x4000);
                                memcpy(&memory[0xC000],rom->get_prg_bank(rom->mmc1shift&0xf),0x4000);
                            } else if (prgmode==3) {
                                memcpy(&memory[0xC000],rom->get_prg_bank((rom->get_prgsize()/0x4000)-1),0x4000);
                                memcpy(&memory[0x8000],rom->get_prg_bank(rom->mmc1shift&0xf),0x4000);
                            } else {
                                rom->mmc1prgloc = rom->mmc1shift &0xe;
                                memcpy(&memory[0x8000],rom->get_prg_bank(rom->mmc1shift&0xe),0x8000);
                            }
                        }
                        rom->mmc1shift = 0x10;
                    }
                }
            }
            }
            break;
    }
    if (!(0x8000<=mem && mem<=0xffff)) {
        *address = value;
    }
}

int8_t CPU::read(int8_t* address) {
    map_memory(&address);
    uint16_t mem = get_addr(address);
    int8_t value = *address;
    switch(mem) { // handle special ppu and apu registers
        case 0x2002:
            *address &= 0x7F;
            ppu->w = 0;
            break;
        case 0x2004: //OAMDATA
            value = ppu->oam[ppu->oam_addr];
            break;
        case 0x2007:
            if (ppu->vblank) {
                ppu->v+=(memory[0x2000]&0x04) ? 0x20 : 0x01;
            }
            //ppu->v %= 0x4000;
            break;
        case 0x4016:
            value = (bool)(inputs&0x80);
            inputs<<=1;
            break;

    }
    return value;
}

void CPU::ins_str(char * write,uint8_t opcode) {
    if (debug_opcodes[opcode]!=nullptr && debug_addr[opcode]!=nullptr) {
        sprintf(write,"0x%02x: %s, %s, PC=$%04x - A=%u - X=%u - Y=%u",
        opcode,
        this->debug_opcodes[opcode],
        this->debug_addr[opcode],
        get_addr(pc),
        accumulator,
        x,
        y);
    } else {
        sprintf(write,"0x%02x: ---",opcode);
    }
}

void CPU::ins_str_mem(char * write,uint8_t* mem,int8_t* arg_ptr) {
    map_memory((int8_t**)&mem);
    map_memory(&arg_ptr);
    uint8_t opcode = mem[0];
    uint16_t a;
    if (ins_size<=3) {
        memcpy(&a,&mem[1],ins_size-1);
    } else {
        a = mem[1];
    }
    if (debug_opcodes[opcode]!=nullptr && debug_addr[opcode]!=nullptr) {
        sprintf(write,"0x%02x: %s, %s $%04x->%04x=%02x, PC=$%04x - A=%02x - X=%02x - Y=%02x - P=%02x",
        opcode,
        this->debug_opcodes[opcode],
        this->debug_addr[opcode],
        a,
        get_addr(arg_ptr),
        (uint8_t)*arg_ptr,
        get_addr(pc),
        (uint8_t)accumulator,
        (uint8_t)x,
        (uint8_t)y,
        flags);
    } else {
        sprintf(write,"0x%02x: ---",opcode);
    }
}

void CPU::map_memory(int8_t** address) {
    uint8_t m = rom->get_mapper();
    int addr = *address-memory;
    switch(m) {
        case 0:
            if (rom->get_prgsize()/0x4000==1) {
                if (0xC000<=addr && addr<=0xFFFF) {
                    *address-=0x4000; //mirror first table
                }
            }
            break;
    }
    if (0x0800<=addr && addr < 0x2000) {
        *address-=addr/0x800*0x800;
    }
}

void CPU::clock() {
    if (emulated_clock_speed()<=CLOCK_SPEED) { //limit clock speed
        ins_size = 1;
        cycles+=2;
        int8_t* ins = pc;
        //map_memory(&ins);
        uint8_t ins_value = read(ins);
        instruction exec = this->opcodes[ins_value]; // get instruction from lookup table
        addressing_mode addr = this->addrmodes[ins_value]; // get addressing mode from another lookup table
        int8_t* arg = &ins[1];
        if (addr!=nullptr) {
            arg = (this->*addr)(&ins[1]); // run addressing mode on raw value from rom
        }
        //map_memory(&arg); // update banks and registers as needed
        if (debug) { //print instruction
            char w[256] = {0};
            ins_str_mem(w,(uint8_t*)ins,arg);
            printf("%s ",w);
            //print stack
            printf("SP: %02x [",sp);
            if (sp!=0xff) {
                printf("%02x",(uint8_t)memory[0x01ff]);
            }
            for (int i=0xfe; i>sp; i--) {
                printf(",%02x",(uint8_t)memory[0x100+i]);
            }
            printf("]\n");
        }
        (this->*exec)(arg); // execute instruction
        ins_num++;
        pc+=ins_size; // increment by instruction size (determined by addressing mode)
        if (recv_nmi) {
            start_nmi();
        }
    }

    
}

int CPU::emulated_clock_speed() {
    if (epoch()!=start) {
        return cycles/(epoch()-start)*1000;
    } else {
        return 0;
    }
}

void CPU::reset() {
    int8_t * res = &memory[RESET];
    printf("Before: %04x\n",get_addr(res));
    map_memory(&res);
    printf("After: %04x\n",get_addr(res));
    printf("%02x %02x\n",*res,*(res+1));
    pc = abs(res);
    //for test purposes: remove this later.
    //pc = &memory[0xc000];
}

void CPU::loadRom(ROM *r) {
    rom = r;
    uint8_t m = rom->get_mapper();
    switch(m) {
        case 0:
            memcpy(&memory[0x8000],rom->prg,rom->get_prgsize());
            break;
        case 1:
            memcpy(&memory[0xc000],rom->get_prg_bank((rom->get_prgsize()/0x4000)-1),0x4000);
            break;
    }
    reset();

}

long long CPU::get_addr(int8_t* ptr) {
    return ptr-memory;
}

bool CPU::get_flag(char flag) {
    switch(flag) {
        case 'C':
            return flags&0x1;
        case 'Z':
            return flags&0x2;
        case 'I':
            return flags&0x4;
        case 'D':
            return flags&0x8; //this flag is disabled on NES 6502
        case 'B':
            return flags&0x10;
        case 'V':
            return flags&0x40;
        case 'N':
            return flags&0x80;
        default:
            return 0;
    }
}

void CPU::set_flag(char flag,bool val) {
    if (val) {
        switch(flag) {
            case 'C':
                flags|=0x01;
                break;
            case 'Z':
                flags|=0x02;
                break;
            case 'I':
                flags|=0x04;
                break;
            case 'D':
                flags|=0x08;
                break;
            case 'B':
                flags|=0x10;
                break;
            case 'V':
                flags|=0x40;
                break;
            case 'N':
                flags|=0x80;
                break;
        }
    } else {
        switch(flag) {
            case 'C':
                flags&=0xFE;
                break;
            case 'Z':
                flags&=0xFD;
                break;
            case 'I':
                flags&=0xFB;
                break;
            case 'D':
                flags&=0xF7;
                break;
            case 'B':
                flags&=0xEF;
                break;
            case 'V':
                flags&=0xBF;
                break;
            case 'N':
                flags&=0x7F;
                break;
        }
    }
}

void CPU::stack_push(int8_t val) {
    memory[0x0100+sp] = val;
    sp--;
}

uint8_t CPU::stack_pull(void) {
    sp++;
    //printf("Top of stack (%02x): %02x\n",sp,(uint8_t)memory[0x0100+sp]);
    return memory[0x0100+sp]; 
}

