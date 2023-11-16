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
        switch(b) {
            case 1:
                this->addrmodes[i] = &zpg;
            case 3:
                this->addrmodes[i] = &abs;
            case 5:
                this->addrmodes[i] = &zpgx;
            case 7:
                this->addrmodes[i] = &absx;
        }
        switch(c) {
            case 0:
                switch(b) {
                    case 0:
                        if (a=0||a==2||a==3) {
                            this->addrmodes[i] = nullptr;
                        } else if (a>4) {
                            this->addrmodes[i] = &imm;
                        } else if (a=1) {
                            this->addrmodes[i] = &abs;
                            this->opcodes[i] = &JSR;
                        }
                        switch(a) {
                            case 0:
                                this->opcodes[i] = &BRK;
                            case 2:
                                this->opcodes[i] = &RTI;
                            case 3:
                                this->opcodes[i] = &RTS;
                            case 5:
                                this->opcodes[i] = &LDY;
                            case 6:
                                this->opcodes[i] = &CPY;
                            case 7:
                                this->opcodes[i] = &CPX;
                        }
                    //continue here
                    case 2:
                        this->addrmodes[i] = nullptr;
                    case 3:
                        if (a==3) {
                            this->addrmodes[i] = &ind;
                        }
                    case 4:
                        this->addrmodes[i] = &rel;
                    case 6:
                        this->addrmodes[i] = nullptr;
                }
            case 1:
                switch(a) {
                    case 0:
                        this->opcodes[i] = &ORA;
                    case 1:
                        this->opcodes[i] = &AND;
                    case 2:
                        this->opcodes[i] = &EOR;
                    case 3:
                        this->opcodes[i] = &ADC;
                    case 4:
                        this->opcodes[i] = &STA;
                    case 5:
                        this->opcodes[i] = &LDA;
                    case 6:
                        this->opcodes[i] = &CMP;
                    case 7:
                        this->opcodes[i] = &SBC;
                }
                switch(b) {
                    case 0:
                        this->addrmodes[i] = &xind;
                    case 2:
                        this->addrmodes[i] = &imm;
                    case 4:
                        this->addrmodes[i] = &indy;
                    case 6:
                        this->addrmodes[i] = &absy;
                }
            case 2:
                switch(a) {
                    case 0:
                        this->opcodes[i] = &ASL;
                    case 1:
                        this->opcodes[i] = &ROL;
                    case 2:
                        this->opcodes[i] = &LSR;
                    case 3:
                        this->opcodes[i] = &ROR;
                    case 4:
                        if (b==2) {
                            this->opcodes[i] = &TXA;
                        } else if (b==6) {
                            this->opcodes[i] = &TXS;
                        } else {
                            this->opcodes[i] = &STX;
                        }
                    case 5:
                        if (b==2) {
                            this->opcodes[i] = &TAX;
                        } else if (b==6) {
                            this->opcodes[i] = &TSX;
                        } else {
                            this->opcodes[i] = &LDX;
                        }
                    case 6:
                        if (b==2) {
                            this->opcodes[i] = &DEX;
                        } else {
                            this->opcodes[i] = &DEC;
                        }
                    case 7:
                        if (b==2) {
                            this->opcodes[i] = &NOP;
                        } else {
                            this->opcodes[i] = &INC;
                        }
                }
                switch(b) {
                    case 0:
                        this->addrmodes[i] = &imm;
                    case 2:
                        this->addrmodes[i] = nullptr;
                    case 5:
                        if (a==4||a==5) {
                            this->addrmodes[i] = &zpgy;
                        }
                    case 6:
                        this->addrmodes[i] = nullptr;
                    case 7:
                        if (a==5) {
                            this->addrmodes[i] = &absy;
                }
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