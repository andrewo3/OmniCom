#include "cpu.h"
#include <stdint.h>

CPU::CPU() {

}
// description of addressing modes:
// https://blogs.oregonstate.edu/ericmorgan/2022/01/21/6502-addressing-modes/
void CPU::instruction(uint8_t* ins) {
    uint8_t a = (ins[0] & 0xE0)>>5;
    uint8_t b = (ins[0] & 0x1C)>>2;
    uint8_t c = ins[0] & 0x3;
    if (c==1) {
        switch(a) {
            case 0: //ORA x,ind
                ORA(ins+1);
        }

    }
}