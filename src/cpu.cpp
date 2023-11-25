#include "rom.h"
#include "cpu.h"
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

void CPU::ins_str_mem(char * write,uint8_t* mem) {
    uint8_t opcode = mem[0];
    uint16_t a;
    memcpy(&a,&mem[1],ins_size-1);
    if (debug_opcodes[opcode]!=nullptr && debug_addr[opcode]!=nullptr) {
        sprintf(write,"0x%02x: %s, %s $%04x, PC=$%04x - A=%u - X=%u - Y=%u",
        opcode,
        this->debug_opcodes[opcode],
        this->debug_addr[opcode],
        a,
        get_addr(pc),
        (uint8_t)accumulator,
        (uint8_t)x,
        (uint8_t)y);
    } else {
        sprintf(write,"0x%02x: ---",opcode);
    }
}

void CPU::map_memory(int8_t** address) {
    /* TODO*/
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
    ins_size = 1;
    int8_t* ins = pc;
    map_memory(&ins);
    instruction exec = this->opcodes[(uint8_t)ins[0]]; // get instruction from lookup table
    addressing_mode addr = this->addrmodes[(uint8_t)ins[0]]; // get addressing mode from another lookup table
    int8_t* arg = &ins[1];
    if (addr!=nullptr) {
        arg = (this->*addr)(&ins[1]); // run addressing mode on raw value from rom
    }
    map_memory(&arg); // update banks and registers as needed
    (this->*exec)(arg); // execute instruction
    if (debug) { //print instruction
        char w[20] = {0};
        ins_str_mem(w,(uint8_t*)ins);
        printf("%s\n",w);
    }
    clocks++;
    pc+=ins_size; // increment by instruction size (determined by addressing mode)
    
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

int8_t CPU::stack_pull(void) {
    sp--;
    return memory[0x0100+sp]; 
}

