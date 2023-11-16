#include "cpu.h"
#include <stdint.h>

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

void CPU::define_opcodes() {
    for (int i=0; i<0xFF; i++) {
        uint8_t a = (i & 0xE0)>>5;
        uint8_t b = (i & 0x1C)>>2;
        uint8_t c = i & 0x3;
        if (c==1) {
            switch(b) {
                case 0:
                    addrmodes[i] = xind;
                case 1:
                    addrmodes[i] = zpg;
                case 2:
                    addrmodes[i] = imm;
                case 3:
                    addrmodes[i] = abs;
                case 4:
                    addrmodes[i] = indy;
                case 5:
                    addrmodes[i] = zpgx;
                case 6:
                    addrmodes[i] = absy;
                case 7:
                    addrmodes[i] = absx;
            }
            switch(a) {
                
            }

        }
    }
}

void CPU::clock(uint8_t* ins) {

}

void CPU::reset() {
    pc = &memory[RESET];
}

uint8_t* CPU::xind(uint8_t* args) {
    return &memory[args[0]+x];
}

uint8_t* CPU::indy(uint8_t* args) {
    uint16_t ind = memory[args[0]] | (memory[args[1]]<<8);
    return &memory[ind+y];
}

uint8_t* CPU::zpg(uint8_t* args) {
    return &memory[args[0]];
}

uint8_t* CPU::zpgx(uint8_t* args) {
    return &memory[args[0]+x];
}

uint8_t* CPU::zpgy(uint8_t* args) {
    return &memory[args[0]+y];
}

uint8_t* CPU::abs(uint8_t* args) {
    return &memory[args[0]|(args[1]<<8)];
}

uint8_t* CPU::absx(uint8_t* args) {
    return &memory[(args[0]|(args[1]<<8))+x];
}

uint8_t* CPU::absy(uint8_t* args) {
    return &memory[(args[0]|(args[1]<<8))+y];
}

uint8_t* CPU::ind(uint8_t* args) {
    return &memory[memory[args[0]] | (memory[args[1]]<<8)];
}

uint8_t* CPU::rel(uint8_t* args) {
    return &memory[get_addr(pc)+(int8_t)args[0]];
}

uint16_t CPU::get_addr(uint8_t* ptr) {
    return ptr-memory;
}

void CPU::ORA(uint8_t* args) {

}