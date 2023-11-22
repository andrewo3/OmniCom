#include "rom.h"
#include "cpu.h"
#include <cstdint>
#include <stdio.h>

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
    reset();
}

CPU::CPU(bool dbug) {
    this->debug = dbug;
    define_opcodes();
    reset();
}

void CPU::ins_str(char * write,uint8_t opcode) {
            if (debug_opcodes[opcode]!=nullptr && debug_addr[opcode]!=nullptr) {
                sprintf(write,"0x%02x: %s, %s, PC=$%04x",opcode,this->debug_opcodes[opcode],this->debug_addr[opcode],get_addr(pc));
            } else {
                sprintf(write,"0x%02x: ---",opcode);
            }
        }

void CPU::map_memory(uint8_t mapper_num,int8_t* address) {
    /* TODO*/
    printf("mappin\n");
}

void CPU::clock() {
    ins_size = 1;
    int8_t* ins = pc;
    if (debug) {
        char w[20] = {0};
        ins_str(w,ins[0]);
        printf("%s\n",w);
    }
    instruction exec = this->opcodes[ins[0]]; // get instruction from lookup table
    addressing_mode addr = this->addrmodes[ins[0]]; // get addressing mode from another lookup table
    int8_t* arg = &ins[1];
    if (addr!=nullptr) {
        arg = (this->*addr)(&ins[1]); // run addressing mode on raw value from rom
    }
    map_memory((*rom).get_mapper(),arg); // update banks and registers as needed
    (this->*exec)(arg); // execute instruction
    pc+=ins_size; // increment by instruction size (determined by addressing mode)
    
}

void CPU::reset() {
    pc = this->abs(&memory[RESET]);
}

void CPU::loadRom(ROM *r) {
    rom = r;
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

