#include "cpu.h"
#include <string>

//Good reference: http://www.6502.org/tutorials/65c816opcodes.html

using namespace SNES;

uint8_t* CPU::PC_rel_long(uint8_t* arg) {
    return memory + PC + *(int16_t*)arg;
}

uint8_t* CPU::stack_rel(uint8_t* arg) {
    return memory + *(int8_t*)arg[0]+SP;
}