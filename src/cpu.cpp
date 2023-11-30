#include "rom.h"
#include "cpu.h"
#include "ppu.h"
#include "util.h"
#include <cstdint>
#include <cstdio>
#include <cstring>

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
    recv_nmi = false;
    uint16_t push = get_addr(pc-1);
    stack_push((uint8_t)(push>>8));
    stack_push((uint8_t)(push&0xff));
    stack_push(flags);

    int8_t * res = &memory[NMI];
    pc = abs(res); 
}

void CPU::write(int8_t* address, int8_t value) {
    map_memory(&address);
    uint16_t mem = get_addr(address); 
    switch(mem) {
        //write to OAMADDR (0x2001) is handled implicitly
        //write to OAMADDR (0x2003) is handled implicitly
        case 0x2004: //write to OAMDATA
            ppu->oam[(uint8_t)memory[0x2003]] = value;
            memory[0x2003]++;
        case 0x2005: //write to PPUSCROLL
            if (!ppu->vram_twice) {
                ppu->scroll_x = (uint8_t)value;
                ppu->vram_twice = 1;
            } else {
                ppu->scroll_y = (uint8_t)value;
                ppu->vram_twice = 0;
            }
        case 0x2006: //write to PPUADDR
            if (!ppu->vram_twice) {
                ppu->vram_addr = (uint16_t)value<<8;
                ppu->vram_twice = 1;
            } else {
                ppu->vram_addr |= (uint8_t)value;
                ppu->vram_addr %= 0x4000;
                ppu->vram_twice = 0;
            }
            break;
        case 0x2007: // write to PPUDATA
            ppu->write(&(ppu->memory[ppu->vram_addr]),value); // write method takes mapper into account
            ppu->vram_addr+=(memory[0x2000]&0x04) ? 0x20 : 0x01;
            ppu->vram_addr %= 0x4000;
            break;

        case 0x4014: //write to OAMDMA
            memcpy(ppu->oam,&memory[(uint16_t)value<<8],256);
            break;
    }
    *address = value;
}

int8_t CPU::read(int8_t* address) {
    map_memory(&address);
    uint16_t mem = get_addr(address);
    int8_t value = *address;
    switch(mem) { // handle special ppu and apu registers
        case 0x2002:
            *address &= 0x7F;
            ppu->vram_twice = 0;
            break;
        case 0x2007:
            ppu->vram_addr+=(memory[0x2000]&0x04) ? 0x20 : 0x01;
            ppu->vram_addr %= 0x4000;
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
        sprintf(write,"0x%02x: %s, %s $%04x->%04x=%02x, PC=$%04x - A=%02x - X=%02x - Y=%02x",
        opcode,
        this->debug_opcodes[opcode],
        this->debug_addr[opcode],
        a,
        get_addr(arg_ptr),
        (uint8_t)*arg_ptr,
        get_addr(pc),
        (uint8_t)accumulator,
        (uint8_t)x,
        (uint8_t)y);
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
            printf("%s\n",w);
        }
        (this->*exec)(arg); // execute instruction
        ins_num++;
        if (recv_nmi) {
            start_nmi();
        } else {
            pc+=ins_size; // increment by instruction size (determined by addressing mode)
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
    map_memory(&res);
    pc = abs(res);
}

void CPU::loadRom(ROM *r) {
    rom = r;
    uint8_t m = rom->get_mapper();
    switch(m) {
        case 0:
            memcpy(&memory[0x8000],rom->prg,rom->get_prgsize());
    }
    reset();

}

uint16_t CPU::get_addr(int8_t* ptr) {
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
                flags|=0x1;
            case 'Z':
                flags|=0x2;
            case 'I':
                flags|=0x4;
            case 'D':
                flags|=0x8;
            case 'B':
                flags|=0x10;
            case 'V':
                flags|=0x40;
            case 'N':
                flags|=0x80;
        }
    } else {
        switch(flag) {
            case 'C':
                flags&=0xFE;
            case 'Z':
                flags&=0xFD;
            case 'I':
                flags&=0xFB;
            case 'D':
                flags&=0xF7;
            case 'B':
                flags&=0xEF;
            case 'V':
                flags&=0xBF;
            case 'N':
                flags&=0x7F;
        }
    }
}

void CPU::stack_push(int8_t val) {
    memory[0x0100+sp] = val;
    sp++;
}

uint8_t CPU::stack_pull(void) {
    sp--;
    //printf("Top of stack (%02x): %02x\n",sp,(uint8_t)memory[0x0100+sp]);
    return memory[0x0100+sp]; 
}

