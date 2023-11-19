#include "rom.h"
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
        int8_t a = (i & 0xE0)>>5;
        int8_t b = (i & 0x1C)>>2;
        int8_t c = i & 0x3;
        switch(b) {
            case 1:
                this->addrmodes[i] = &CPU::zpg;
            case 3:
                this->addrmodes[i] = &CPU::abs;
            case 5:
                this->addrmodes[i] = &CPU::zpgx;
            case 7:
                this->addrmodes[i] = &CPU::absx;
        }
        switch(c) {
            case 0:
                switch(b) {
                    case 0:
                        if (a=0||a==2||a==3) {
                            this->addrmodes[i] = nullptr;
                        } else if (a>4) {
                            this->addrmodes[i] = &CPU::imm;
                        } else if (a=1) {
                            this->addrmodes[i] = &CPU::abs;
                            this->opcodes[i] = &CPU::JSR;
                        }
                        switch(a) {
                            case 0:
                                this->opcodes[i] = &CPU::BRK;
                            case 2:
                                this->opcodes[i] = &CPU::RTI;
                            case 3:
                                this->opcodes[i] = &CPU::RTS;
                            case 5:
                                this->opcodes[i] = &CPU::LDY;
                            case 6:
                                this->opcodes[i] = &CPU::CPY;
                            case 7:
                                this->opcodes[i] = &CPU::CPX;
                        }

                    case 1:
                        switch(a) {
                            case 1:
                                this->opcodes[i] = &CPU::BIT;
                            case 4:
                                this->opcodes[i] = &CPU::STY;
                            case 5:
                                this->opcodes[i] = &CPU::LDY;
                            case 6:
                                this->opcodes[i] = &CPU::CPY;
                            case 7:
                                this->opcodes[i] = &CPU::CPX;
                        }
                    case 2:
                        this->addrmodes[i] = nullptr;
                        switch(a) {
                            case 0:
                                this->opcodes[i] = &CPU::PHP;
                            case 1:
                                this->opcodes[i] = &CPU::PLP;
                            case 2:
                                this->opcodes[i] = &CPU::PHA;
                            case 3:
                                this->opcodes[i] = &CPU::PLA;
                            case 4:
                                this->opcodes[i] = &CPU::DEY;
                            case 5:
                                this->opcodes[i] = &CPU::TAY;
                            case 6:
                                this->opcodes[i] = &CPU::INY;
                            case 7:
                                this->opcodes[i] = &CPU::INX;

                        }
                    case 3:
                        switch(a) {
                            case 1:
                                this->opcodes[i] = &CPU::BIT;
                            case 2:
                                this->opcodes[i] = &CPU::JMP;
                            case 3:
                                this->opcodes[i] = &CPU::JMP;
                                this->addrmodes[i] = &CPU::ind;
                            case 4:
                                this->opcodes[i] = &CPU::STY;
                            case 5:
                                this->opcodes[i] = &CPU::LDY;
                            case 6:
                                this->opcodes[i] = &CPU::CPY;
                            case 7:
                                this->opcodes[i] = &CPU::CPX;
                        }
                    case 4:
                        this->addrmodes[i] = &CPU::rel;
                        switch(a) {
                            case 0:
                                this->opcodes[i] = &CPU::BPL;
                            case 1:
                                this->opcodes[i] = &CPU::BMI;
                            case 2:
                                this->opcodes[i] = &CPU::BVC;
                            case 3:
                                this->opcodes[i] = &CPU::BVS;
                            case 4:
                                this->opcodes[i] = &CPU::BCC;
                            case 5:
                                this->opcodes[i] = &CPU::BCS;
                            case 6:
                                this->opcodes[i] = &CPU::BNE;
                            case 7:
                                this->opcodes[i] = &CPU::BEQ;
                        }
                    case 5:
                        switch(a) {
                            case 4:
                                this->opcodes[i] = &CPU::STY;
                            case 5:
                                this->opcodes[i] = &CPU::LDY;
                        }
                    case 6:
                        this->addrmodes[i] = nullptr;
                        switch(a) {
                            case 0:
                                this->opcodes[i] = &CPU::CLC;
                            case 1:
                                this->opcodes[i] = &CPU::SEC;
                            case 2:
                                this->opcodes[i] = &CPU::CLI;
                            case 3:
                                this->opcodes[i] = &CPU::SEI;
                            case 4:
                                this->opcodes[i] = &CPU::TYA;
                            case 5:
                                this->opcodes[i] = &CPU::CLV;
                            case 6:
                                this->opcodes[i] = &CPU::CLD;
                            case 7:
                                this->opcodes[i] = &CPU::SED;
                        }
                    case 7:
                        if (a==5) {
                            this->opcodes[i] = &CPU::LDY;
                        }
                }
            case 1:
                switch(a) {
                    case 0:
                        this->opcodes[i] = &CPU::ORA;
                    case 1:
                        this->opcodes[i] = &CPU::AND;
                    case 2:
                        this->opcodes[i] = &CPU::EOR;
                    case 3:
                        this->opcodes[i] = &CPU::ADC;
                    case 4:
                        this->opcodes[i] = &CPU::STA;
                    case 5:
                        this->opcodes[i] = &CPU::LDA;
                    case 6:
                        this->opcodes[i] = &CPU::CMP;
                    case 7:
                        this->opcodes[i] = &CPU::SBC;
                }
                switch(b) {
                    case 0:
                        this->addrmodes[i] = &CPU::xind;
                    case 2:
                        this->addrmodes[i] = &CPU::imm;
                    case 4:
                        this->addrmodes[i] = &CPU::indy;
                    case 6:
                        this->addrmodes[i] = &CPU::absy;
                }
            case 2:
                switch(a) {
                    case 0:
                        this->opcodes[i] = &CPU::ASL;
                    case 1:
                        this->opcodes[i] = &CPU::ROL;
                    case 2:
                        this->opcodes[i] = &CPU::LSR;
                    case 3:
                        this->opcodes[i] = &CPU::ROR;
                    case 4:
                        if (b==2) {
                            this->opcodes[i] = &CPU::TXA;
                        } else if (b==6) {
                            this->opcodes[i] = &CPU::TXS;
                        } else {
                            this->opcodes[i] = &CPU::STX;
                        }
                    case 5:
                        if (b==2) {
                            this->opcodes[i] = &CPU::TAX;
                        } else if (b==6) {
                            this->opcodes[i] = &CPU::TSX;
                        } else {
                            this->opcodes[i] = &CPU::LDX;
                        }
                    case 6:
                        if (b==2) {
                            this->opcodes[i] = &CPU::DEX;
                        } else {
                            this->opcodes[i] = &CPU::DEC;
                        }
                    case 7:
                        if (b==2) {
                            this->opcodes[i] = &CPU::NOP;
                        } else {
                            this->opcodes[i] = &CPU::INC;
                        }
                }
                switch(b) {
                    case 0:
                        this->addrmodes[i] = &CPU::imm;
                    case 2:
                        if (a<4) {
                            this->addrmodes[i] = &CPU::acc;
                        } else {
                            this->addrmodes[i] = nullptr;
                        }
                    case 5:
                        if (a==4||a==5) {
                            this->addrmodes[i] = &CPU::zpgy;
                        }
                    case 6:
                        this->addrmodes[i] = nullptr;
                    case 7:
                        if (a==5) {
                            this->addrmodes[i] = &CPU::absy;
                        }
                }
        }   
        
    }
}

void CPU::map_memory(uint8_t mapper_num) {
    // TODO
}

void CPU::clock() {
    ins_size = 1;
    map_memory((*rom).get_mapper()); // update banks and registers
    int8_t* ins = pc;
    instruction exec = this->opcodes[ins[0]]; // get instruction from lookup table
    addressing_mode addr = this->addrmodes[ins[0]]; // get addressing mode from another lookup table
    int8_t* arg = (this->*addr)(&ins[1]); // run addressing mode on raw value from rom
    (this->*exec)(arg); // execute instruction
    pc+=ins_size; // increment by instruction size (determined by addressing mode)
    
}

void CPU::reset() {
    pc = this->abs(&memory[RESET]);
}

void CPU::loadRom(ROM *r) {
    rom = r;
}

int8_t* CPU::xind(int8_t* args) {
    ins_size = 2;
    return &memory[args[0]+x];
}

int8_t* CPU::indy(int8_t* args) {
    ins_size = 2;
    uint16_t ind = memory[args[0]] | (memory[args[1]]<<8);
    return &memory[ind+y];
}

int8_t* CPU::zpg(int8_t* args) {
    ins_size = 2;
    return &memory[args[0]];
}

int8_t* CPU::zpgx(int8_t* args) {
    ins_size = 2;
    return &memory[args[0]+x];
}

int8_t* CPU::zpgy(int8_t* args) {
    ins_size = 2;
    return &memory[(uint8_t)(args[0]+y)];
}

int8_t* CPU::abs(int8_t* args) {
    ins_size = 3;
    return &memory[(uint8_t)(args[0]|(args[1]<<8))];
}

int8_t* CPU::absx(int8_t* args) {
    ins_size = 3;
    return &memory[(uint8_t)((args[0]|(args[1]<<8))+x)];
}

int8_t* CPU::absy(int8_t* args) {
    ins_size = 3;
    return &memory[(uint8_t)((args[0]|(args[1]<<8))+y)];
}

int8_t* CPU::ind(int8_t* args) {
    ins_size = 3;
    return &memory[(uint8_t)(memory[(uint8_t)args[0]] | (memory[(uint8_t)args[1]]<<8))];
}

int8_t* CPU::rel(int8_t* args) {
    ins_size = 2;
    return &memory[this->get_addr(pc)+args[0]];
}

int8_t* CPU::acc(int8_t* args) {
    return &accumulator;
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

void CPU::ADC(int8_t* args) {
    uint16_t unwrapped = (uint8_t)accumulator+
                        (uint8_t)*args+
                        this->get_flag('C');
    this->set_flag('C',unwrapped>0xFF);
    this->set_flag('V',!((accumulator^*args)&0x80) && ((accumulator^unwrapped) & 0x80));
    this->set_flag('Z',!accumulator);
    this->set_flag('N',accumulator&0x80);
}

void CPU::AND(int8_t* args) {
    accumulator = accumulator&*args;
    this->set_flag('Z',!accumulator);
    this->set_flag('N',accumulator&0x80);
}

void CPU::ASL(int8_t* args) {
    uint16_t result = *args<<1;
    *args = result&0xff;
    this->set_flag('C',result&0x100);
    this->set_flag('Z',!accumulator);
    this->set_flag('N',result&0x80);
}

void CPU::BCC(int8_t* args) {
    if (!get_flag('C')) {
        pc = args-ins_size;
    }
}

void CPU::BCS(int8_t* args) {
    if (get_flag('C')) {
        pc = args-ins_size;
    }
}

void CPU::BEQ(int8_t* args) {
    if (get_flag('Z')) {
        pc = args-ins_size;
    }
}

void CPU::BIT(int8_t* args) {
    int8_t test = accumulator & *args;
    this->set_flag('Z',test==0);
    this->set_flag('V',*args&0x40);
    this->set_flag('N',*args&0x80);
}

void CPU::BMI(int8_t* args) {
    if (get_flag('N')) {
        pc = args-ins_size;
    }
}

void CPU::BNE(int8_t* args) {
    if (!get_flag('Z')) {
        pc = args-ins_size;   
    }
}

void CPU::BPL(int8_t* args) {
    if (!get_flag('N')) {
        pc = args-ins_size;
    }
}

void CPU::BRK(int8_t* args) {
    // push high byte first
    uint16_t last_ptr = get_addr(pc+ins_size);
    stack_push((int8_t)(last_ptr>>8));
    stack_push((int8_t)(last_ptr&0xff));
    stack_push(flags);
    pc = this->abs(&memory[0xFFFE])-ins_size;
}

void CPU::BVC(int8_t* args) {
    if (!get_flag('C')) {
        pc = args-ins_size;
    }
}

void CPU::BVS(int8_t* args) {
    if(get_flag('V')) {
        pc = args-ins_size;
    }
}

void CPU::CLC(int8_t* args) {
    this->set_flag('C',0);
}

void CPU::CLD(int8_t* args) {
    this->set_flag('D',0);
}

void CPU::CLI(int8_t* args) {
    this->set_flag('I',0);
}

void CPU::CLV(int8_t* args) {
    this->set_flag('V',0);
}

void CPU::CMP(int8_t* args) {
    this->set_flag('C',accumulator>=*args);
    this->set_flag('Z',accumulator==*args);
    this->set_flag('N',(accumulator-*args)&0x80);
}

void CPU::CPX(int8_t* args) {
    this->set_flag('C',x>=*args);
    this->set_flag('Z',x==*args);
    this->set_flag('N',(x-*args)&0x80);
}
void CPU::CPY(int8_t* args) {
    this->set_flag('C',y>=*args);
    this->set_flag('Z',y==*args);
    this->set_flag('N',(y-*args)&0x80);
}

void CPU::DEC(int8_t* args) {
    *args-=1;
    this->set_flag('Z',!*args);
    this->set_flag('N',*args&0x80);
}

void CPU::DEX(int8_t* args) {
    x-=1;
    this->set_flag('Z',!x);
    this->set_flag('N',x&0x80);
}

void CPU::DEY(int8_t* args) {
    y-=1;
    this->set_flag('Z',!y);
    this->set_flag('N',y&0x80);
}

void CPU::EOR(int8_t* args) {
    accumulator = accumulator ^ *args;
    this->set_flag('Z',!accumulator);
    this->set_flag('N',accumulator&0x80);
}

void CPU::INC(int8_t* args) {
    *args+=1;
    this->set_flag('Z',!*args);
    this->set_flag('N',*args&0x80);
}

void CPU::INX(int8_t* args) {
    x+=1;
    this->set_flag('Z',!x);
    this->set_flag('N',x&0x80);
}

void CPU::INY(int8_t* args) {
    y+=1;
    this->set_flag('Z',!y);
    this->set_flag('N',y&0x80);
}

void CPU::JMP(int8_t* args) {
    pc = args-ins_size;
}

void CPU::JSR(int8_t* args) {
    uint16_t push = get_addr(pc+ins_size-1);
    stack_push((int8_t)(push>>8));
    stack_push((int8_t)(push&0xff));
    pc = args-ins_size;
}

void CPU::LDA(int8_t* args) {
    accumulator = *args;
    this->set_flag('Z',!accumulator);
    this->set_flag('N',accumulator&0x80);
}

void CPU::LDX(int8_t* args) {
    x = *args;
    this->set_flag('Z',!x);
    this->set_flag('N',x&0x80);
}

void CPU::LDY(int8_t* args) {
    y = *args;
    this->set_flag('Z',!y);
    this->set_flag('N',y&0x80);
}

void CPU::LSR(int8_t* args) {
    this->set_flag('C',*args&1);
    uint16_t result = *args>>1;
    *args = result&0xff;
    this->set_flag('Z',!*args);
    this->set_flag('N',result&0x80);
}

void CPU::NOP(int8_t* args) {
    // does nothing
}

void CPU::ORA(int8_t* args) {
    accumulator|=*args;
    this->set_flag('Z',!accumulator);
    this->set_flag('N',accumulator&0x80);
}

void CPU::PHA(int8_t* args) {
    stack_push(accumulator);
}

void CPU::PHP(int8_t* args) {
    stack_push(flags);
}

void CPU::PLA(int8_t* args) {
    accumulator = stack_pull();
}

void CPU::PLP(int8_t* args) {
    flags = stack_pull();
}

void CPU::ROL(int8_t* args) {
    int8_t changed = *args<<1;
    changed = this->get_flag('C')?(changed|1):(changed&0xFE);
    this->set_flag('C',*args&0x80);
    *args = changed;
    this->set_flag('N',*args&0x80);
    this->set_flag('Z',!accumulator);
}

void CPU::ROR(int8_t* args) {
    int8_t changed = *args>>1;
    changed = this->get_flag('C')?(changed|0x80):(changed&0x7F);
    this->set_flag('C',*args&1);
    *args = changed;
    this->set_flag('N',*args&0x80);
    this->set_flag('Z',!accumulator);
}

void CPU::RTI(int8_t* args) {
    flags = stack_pull();
    uint16_t new_pc = stack_pull();
    new_pc |=stack_pull()<<8;
    pc = &memory[new_pc];
}

void CPU::RTS(int8_t* args) {
    uint16_t new_pc = stack_pull();
    new_pc |=stack_pull()<<8;
    pc = &memory[new_pc];
}

void CPU::SBC(int8_t* args) {
    uint16_t unwrapped = (uint8_t)accumulator+
                        ~(uint8_t)*args+
                        this->get_flag('C');
    this->set_flag('C',!(unwrapped>0xFF));
    this->set_flag('V',!((accumulator^*args)&0x80) && ((accumulator^unwrapped) & 0x80));
    this->set_flag('Z',!accumulator);
    this->set_flag('N',accumulator&0x80);
}

void CPU::SEC(int8_t* args) {
    this->set_flag('C',1);
}

void CPU::SED(int8_t* args) {
    this->set_flag('D',1);
}

void CPU::SEI(int8_t* args) {
    this->set_flag('I',1);
}

void CPU::STA(int8_t* args) {
    *args = accumulator;
}

void CPU::STX(int8_t* args) {
    *args = x;
}

void CPU::STY(int8_t* args) {
    *args = y;
}

void CPU::TAX(int8_t* args) {
    x = accumulator;
}

void CPU::TAY(int8_t* args) {
    y = accumulator;
}

void CPU::TSX(int8_t* args) {
    x = (int8_t)sp;
}

void CPU::TXA(int8_t* args) {
    accumulator = x;
}

void CPU::TXS(int8_t* args) {
    sp = (uint8_t)x;
}

void CPU::TYA(int8_t* args) {
    accumulator = y;
}