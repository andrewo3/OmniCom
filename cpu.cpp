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

                    case 1:
                        switch(a) {
                            case 1:
                                this->opcodes[i] = &BIT;
                            case 4:
                                this->opcodes[i] = &STY;
                            case 5:
                                this->opcodes[i] = &LDY;
                            case 6:
                                this->opcodes[i] = &CPY;
                            case 7:
                                this->opcodes[i] = &CPX;
                        }
                    case 2:
                        this->addrmodes[i] = nullptr;
                        switch(a) {
                            case 0:
                                this->opcodes[i] = &PHP;
                            case 1:
                                this->opcodes[i] = &PLP;
                            case 2:
                                this->opcodes[i] = &PHA;
                            case 3:
                                this->opcodes[i] = &PLA;
                            case 4:
                                this->opcodes[i] = &DEY;
                            case 5:
                                this->opcodes[i] = &TAY;
                            case 6:
                                this->opcodes[i] = &INY;
                            case 7:
                                this->opcodes[i] = &INX;

                        }
                    case 3:
                        switch(a) {
                            case 1:
                                this->opcodes[i] = &BIT;
                            case 2:
                                this->opcodes[i] = &JMP;
                            case 3:
                                this->opcodes[i] = &JMP;
                                this->addrmodes[i] = &ind;
                            case 4:
                                this->opcodes[i] = &STY;
                            case 5:
                                this->opcodes[i] = &LDY;
                            case 6:
                                this->opcodes[i] = &CPY;
                            case 7:
                                this->opcodes[i] = &CPX;
                        }
                    case 4:
                        this->addrmodes[i] = &rel;
                        switch(a) {
                            case 0:
                                this->opcodes[i] = &BPL;
                            case 1:
                                this->opcodes[i] = &BMI;
                            case 2:
                                this->opcodes[i] = &BVC;
                            case 3:
                                this->opcodes[i] = &BVS;
                            case 4:
                                this->opcodes[i] = &BCC;
                            case 5:
                                this->opcodes[i] = &BCS;
                            case 6:
                                this->opcodes[i] = &BNE;
                            case 7:
                                this->opcodes[i] = &BEQ;
                        }
                    case 5:
                        switch(a) {
                            case 4:
                                this->opcodes[i] = &STY;
                            case 5:
                                this->opcodes[i] = &LDY;
                        }
                    case 6:
                        this->addrmodes[i] = nullptr;
                        switch(a) {
                            case 0:
                                this->opcodes[i] = &CLC;
                            case 1:
                                this->opcodes[i] = &SEC;
                            case 2:
                                this->opcodes[i] = &CLI;
                            case 3:
                                this->opcodes[i] = &SEI;
                            case 4:
                                this->opcodes[i] = &TYA;
                            case 5:
                                this->opcodes[i] = &CLV;
                            case 6:
                                this->opcodes[i] = &CLD;
                            case 7:
                                this->opcodes[i] = &SED;
                        }
                    case 7:
                        if (a==5) {
                            this->opcodes[i] = &LDY;
                        }
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
                        if (a<4) {
                            this->addrmodes[i] = &acc;
                        } else {
                            this->addrmodes[i] = nullptr;
                        }
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

void CPU::clock(int8_t* ins) {
    ins_size = 1;
    instruction exec = this->opcodes[ins[0]]; // get instruction from lookup table
    addressing_mode addr = this->addrmodes[ins[0]]; // get addressing mode from another lookup table
    int8_t* arg = (this->*addr)(&ins[1]); // run addressing mode on raw value from rom
    (this->*exec)(arg); // execute instruction
    pc+=ins_size; // increment by instruction size (determined by addressing mode)
    
}

void CPU::reset() {
    pc = this->abs(&memory[RESET]);
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

void CPU::loadRom(ROM rom) {
    rom.prg;
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