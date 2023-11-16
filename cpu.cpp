#include "cpu.h"
#include <stdint.h>


CPU::CPU() {
    addressing_mode lookup[8] = {
        
        
    };
}
// description of addressing modes:
// https://blogs.oregonstate.edu/ericmorgan/2022/01/21/6502-addressing-modes/
//Addressing modes more in depth: https://wiki.cdot.senecacollege.ca/wiki/6502_Addressing_Modes
//Flags: https://www.nesdev.org/wiki/Status_flags
//Memory Map: https://www.nesdev.org/wiki/CPU_memory_map
//Opcode Table: https://www.masswerk.at/6502/6502_instruction_set.html#layout
//Instruction Descriptions: https://www.nesdev.org/obelisk-6502-guide/reference.html
void CPU::instruction(uint8_t* ins) {
    uint8_t a = (ins[0] & 0xE0)>>5;
    uint8_t b = (ins[0] & 0x1C)>>2;
    uint8_t c = ins[0] & 0x3;
    if (c==1) {
        uint8_t* addr;
        switch(b) {
            case 0: //X, ind
                addr = xind(&ins[1]);
            case 1: //zpg
                addr = zpg(&ins[1]);
            case 2: //#
                addr = &ins[1];
            case 3: //abs
                addr = abs(&ins[1]);
            case 4: //ind, y
                addr = indy(&ins[1]);
            case 5: //zpg,X
                addr = zpgx(&ins[1]);
            case 6: //abs,y
                addr = absy(&ins[1]);
            case 7:
                addr = absreg(&ins[1],&y);
        }

    }
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
    return &memory[pc+(int8_t)args[0]];
}

void CPU::ORA(uint8_t* args) {

}