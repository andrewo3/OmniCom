#include "rom.h"
#include "cpu.h"
#include <cstdint>
#include <cstdio>


//--addressing modes--

int8_t* CPU::xind(int8_t* args) {
    ins_size = 2;
    cycles += 4;
    uint8_t* u = (uint8_t*)&memory[(uint8_t)read(args)+(uint8_t)x];
    uint8_t u_val = read((int8_t*)u);
    uint16_t ind = (uint8_t)read(&memory[u_val]) | (uint16_t)(((uint8_t)read(&memory[u_val+1]))<<8);
    return &memory[ind];
}

int8_t* CPU::indy(int8_t* args) {
    uint8_t* u = (uint8_t*)args;
    ins_size = 2;
    cycles += 3;
    uint8_t u_val = read((int8_t*)u);
    uint16_t ind = (uint8_t)read(&memory[u_val]) | (uint16_t)(((uint8_t)read(&memory[u_val+1]))<<8);
    return &memory[ind+(uint8_t)y];
}

int8_t* CPU::zpg(int8_t* args) {
    ins_size = 2;
    cycles += 1;
    return &memory[(uint8_t)read(args)];
}

int8_t* CPU::zpgx(int8_t* args) {
    ins_size = 2;
    cycles += 2;
    return &memory[(uint8_t)((uint8_t)read(args)+(uint8_t)x)];
}

int8_t* CPU::zpgy(int8_t* args) {
    ins_size = 2;
    cycles += 2;
    return &memory[(uint8_t)((uint8_t)read(args)+(uint8_t)y)];
}

int8_t* CPU::abs(int8_t* args) {
    ins_size = 3;
    cycles += 2;
    return &memory[(uint16_t)(((uint16_t)(read(args)&0xff))|((uint16_t)(read(args+1)&0xff)<<8))];
}

int8_t* CPU::absx(int8_t* args) {
    ins_size = 3;
    cycles += 2;
    uint8_t u = x;
    return &memory[(uint16_t)((((uint8_t)(read(args)&0xff))|((uint16_t)(read(args+1)&0xff)<<8))+(uint16_t)u)];
}

int8_t* CPU::absy(int8_t* args) {
    ins_size = 3;
    cycles += 2;
    uint8_t u = y;
    return &memory[(uint16_t)((((uint8_t)(read(args)&0xff))|((uint16_t)(read(args+1)&0xff)<<8))+(uint16_t)u)];
}

int8_t* CPU::ind(int8_t* args) {
    map_memory(&args);
    ins_size = 3;
    cycles += 3;
    uint16_t* full = (uint16_t*)args;
    return &memory[(uint16_t)((read(&memory[full[0]])&0xff) | ((uint16_t)(read(&memory[full[0]+1])&0xff)<<8))];
}

int8_t* CPU::rel(int8_t* args) {
    ins_size = 2;
    return &memory[this->get_addr(pc)+read(args)];
}

int8_t* CPU::acc(int8_t* args) {
    return &accumulator;
}

//--instruction set--

void CPU::ADC(int8_t* args) {
    uint8_t r = read(args);
    uint16_t unwrapped = (uint8_t)accumulator+
                        r+
                        this->get_flag('C');
    this->set_flag('C',unwrapped>0xFF);
    this->set_flag('V',!((accumulator^r)&0x80) && ((accumulator^unwrapped) & 0x80));
    accumulator = (int8_t)unwrapped;
    this->set_flag('Z',!accumulator);
    this->set_flag('N',accumulator&0x80);
}

void CPU::AND(int8_t* args) {
    uint8_t r = read(args);
    accumulator = accumulator&r;
    this->set_flag('Z',!accumulator);
    this->set_flag('N',accumulator&0x80);
}

void CPU::ASL(int8_t* args) {
    uint8_t r = read(args);
    uint16_t result = r<<1;
    write(args,result&0xff);
    cycles += 2;
    this->set_flag('C',result&0x100);
    this->set_flag('Z',!accumulator);
    this->set_flag('N',result&0x80);
}

void CPU::BCC(int8_t* args) {
    if (!get_flag('C')) {
        pc = args;
    }
}

void CPU::BCS(int8_t* args) {
    if (get_flag('C')) {
        pc = args;
    }
}

void CPU::BEQ(int8_t* args) {
    if (get_flag('Z')) {
        pc = args;
    }
}

void CPU::BIT(int8_t* args) {
    uint8_t r = read(args);
    int8_t test = accumulator & r;
    this->set_flag('Z',!test);
    this->set_flag('V',r&0x40);
    this->set_flag('N',r&0x80);
}

void CPU::BMI(int8_t* args) {
    if (get_flag('N')) {
        pc = args;
    }
}

void CPU::BNE(int8_t* args) {
    if (!get_flag('Z')) {
        pc = args;   
    }
}

void CPU::BPL(int8_t* args) {
    if (!get_flag('N')) {
        pc = args;
    }
}

void CPU::BRK(int8_t* args) {
    // push high byte first
    uint16_t last_ptr = get_addr(pc+ins_size);
    stack_push((int8_t)(last_ptr>>8));
    stack_push((int8_t)(last_ptr&0xff));
    stack_push(flags);
    cycles += 5;
    pc = this->abs(&memory[IRQ])-ins_size;
}

void CPU::BVC(int8_t* args) {
    if (!get_flag('C')) {
        pc = args;
    }
}

void CPU::BVS(int8_t* args) {
    if(get_flag('V')) {
        pc = args;
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
    uint8_t r = read(args);
    this->set_flag('C',(uint8_t)accumulator>=r);
    this->set_flag('Z',(uint8_t)accumulator==r);
    this->set_flag('N',((uint8_t)accumulator-r)&0x80);
}

void CPU::CPX(int8_t* args) {
    uint8_t r = read(args);
    this->set_flag('C',(uint8_t)x>=r);
    this->set_flag('Z',(uint8_t)x==r);
    this->set_flag('N',((uint8_t)x-r)&0x80);
}
void CPU::CPY(int8_t* args) {
    uint8_t r = read(args);
    this->set_flag('C',(uint8_t)y>=r);
    this->set_flag('Z',(uint8_t)y==r);
    this->set_flag('N',((uint8_t)y-r)&0x80);
}

void CPU::DEC(int8_t* args) {
    uint8_t r = read(args);
    r--;
    write(args,r);
    cycles += 2;
    this->set_flag('Z',!r);
    this->set_flag('N',r&0x80);
}

void CPU::DEX(int8_t* args) {
    x--;
    this->set_flag('Z',!x);
    this->set_flag('N',x&0x80);
}

void CPU::DEY(int8_t* args) {
    y--;
    this->set_flag('Z',!y);
    this->set_flag('N',y&0x80);
}

void CPU::EOR(int8_t* args) {
    uint8_t r = read(args);
    accumulator ^= r;
    this->set_flag('Z',!accumulator);
    this->set_flag('N',accumulator&0x80);
}

void CPU::INC(int8_t* args) {
    uint8_t r = read(args);
    r++;
    write(args,r);
    cycles += 2;
    this->set_flag('Z',!r);
    this->set_flag('N',r&0x80);
}

void CPU::INX(int8_t* args) {
    x++;
    this->set_flag('Z',!x);
    this->set_flag('N',x&0x80);
}

void CPU::INY(int8_t* args) {
    y++;
    this->set_flag('Z',!y);
    this->set_flag('N',y&0x80);
}

void CPU::JMP(int8_t* args) {
    pc = args-ins_size;
}

void CPU::JSR(int8_t* args) {
    uint16_t push = get_addr(pc+ins_size-1);
    stack_push((uint8_t)(push>>8));
    stack_push((uint8_t)(push&0xff));
    pc = args-ins_size;
}

void CPU::LDA(int8_t* args) {
    uint8_t r = read(args);
    accumulator = r;
    this->set_flag('Z',!accumulator);
    this->set_flag('N',accumulator&0x80);
}

void CPU::LDX(int8_t* args) {
    uint8_t r = read(args);
    x = r;
    this->set_flag('Z',!x);
    this->set_flag('N',x&0x80);
}

void CPU::LDY(int8_t* args) {
    uint8_t r = read(args);
    y = r;
    this->set_flag('Z',!y);
    this->set_flag('N',y&0x80);
}

void CPU::LSR(int8_t* args) {
    uint8_t r = read(args);
    this->set_flag('C',r&1);
    uint8_t result = r>>1;
    write(args,result&0xff);
    cycles += 2;
    this->set_flag('Z',!accumulator);
    this->set_flag('N',result&0x80);
}

void CPU::NOP(int8_t* args) {
    // does nothing
}

void CPU::ORA(int8_t* args) {
    uint8_t r = read(args);
    accumulator|=r;
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
    this->set_flag('Z',!accumulator);
    this->set_flag('N',accumulator&0x80);
}

void CPU::PLP(int8_t* args) {
    flags = stack_pull();
}

void CPU::ROL(int8_t* args) {
    uint8_t r = read(args);
    uint8_t changed = (r<<1)&0xff;
    changed |= this->get_flag('C')&1;
    this->set_flag('C',r&0x80);
    write(args,changed);
    cycles += 2;
    this->set_flag('N',changed&0x80);
    this->set_flag('Z',!accumulator);
}

void CPU::ROR(int8_t* args) {
    uint8_t r = read(args);
    uint8_t changed = (r>>1)&0xff;
    changed |= (this->get_flag('C')<<7);
    this->set_flag('C',r&1);
    write(args,changed);
    cycles += 2;
    this->set_flag('N',changed&0x80);
    this->set_flag('Z',!accumulator);
}

void CPU::RTI(int8_t* args) {
    flags = stack_pull();
    set_flag('I',false);
    uint16_t new_pc = stack_pull();
    new_pc |=stack_pull()<<8;
    cycles += 4;
    pc = &memory[new_pc];
}

void CPU::RTS(int8_t* args) {
    uint16_t new_pc = (uint8_t)(stack_pull());
    new_pc |=((uint8_t)stack_pull())<<8;
    cycles += 4;
    pc = &memory[new_pc];
}

void CPU::SBC(int8_t* args) {
    uint8_t r = read(args);
    uint16_t unwrapped = (uint8_t)accumulator+
                        (255-r)+
                        this->get_flag('C');
    this->set_flag('C',!(unwrapped>0xFF));
    this->set_flag('V',!((accumulator^r)&0x80) && ((accumulator^unwrapped) & 0x80));
    accumulator = (int8_t)unwrapped;
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
    write(args,accumulator);
}

void CPU::STX(int8_t* args) {
    write(args,x);
}

void CPU::STY(int8_t* args) {
    write(args,y);
}

void CPU::TAX(int8_t* args) {
    x = accumulator;
    this->set_flag('Z',!x);
    this->set_flag('N',x&0x80);
}

void CPU::TAY(int8_t* args) {
    y = accumulator;
    this->set_flag('Z',!y);
    this->set_flag('N',y&0x80);

}

void CPU::TSX(int8_t* args) {
    x = (int8_t)sp;
    this->set_flag('Z',!x);
    this->set_flag('N',x&0x80);
}

void CPU::TXA(int8_t* args) {
    accumulator = x;
    this->set_flag('Z',!accumulator);
    this->set_flag('N',accumulator&0x80);
}

void CPU::TXS(int8_t* args) {
    sp = (uint8_t)x;
}

void CPU::TYA(int8_t* args) {
    accumulator = y;
    this->set_flag('Z',!accumulator);
    this->set_flag('N',accumulator&0x80);
}

//--defining instruction lookup table

void CPU::define_opcodes() {
    for (int i=0; i<=0xff; i++) {
        uint8_t a = (i & 0xE0)>>5;
        uint8_t b = (i & 0x1C)>>2;
        uint8_t c = i & 0x3;
        switch(b) {
            case 1:
                this->addrmodes[i] = &CPU::zpg;
                this->debug_addr[i] = "zpg";
                break;
            case 3:
                this->addrmodes[i] = &CPU::abs;
                this->debug_addr[i] = "abs";
                break;
            case 5:
                this->addrmodes[i] = &CPU::zpgx;
                this->debug_addr[i] = "zpgx";
                break;
            case 7:
                this->addrmodes[i] = &CPU::absx;
                this->debug_addr[i] = "absx";
                break;
        }
        switch(c) {
            case 0:
                switch(b) {
                    case 0:
                        if (a==0||a==2||a==3) {
                            this->addrmodes[i] = nullptr;
                            this->debug_addr[i] = "impl";
                        } else if (a>4) {
                            this->addrmodes[i] = &CPU::imm;
                            this->debug_addr[i] = "imm";
                        } else if (a==1) {
                            this->addrmodes[i] = &CPU::abs;
                            this->opcodes[i] = &CPU::JSR;
                            this->debug_addr[i] = "abs";
                            this->debug_opcodes[i] = "JSR";
                        }
                        switch(a) {
                            case 0:
                                this->opcodes[i] = &CPU::BRK;
                                this->debug_opcodes[i] = "BRK";
                                break;
                            case 2:
                                this->opcodes[i] = &CPU::RTI;
                                this->debug_opcodes[i] = "RTI";
                                break;
                            case 3:
                                this->opcodes[i] = &CPU::RTS;
                                this->debug_opcodes[i] = "RTS";
                                break;
                            case 5:
                                this->opcodes[i] = &CPU::LDY;
                                this->debug_opcodes[i] = "LDY";
                                break;
                            case 6:
                                this->opcodes[i] = &CPU::CPY;
                                this->debug_opcodes[i] = "CPY";
                                break;
                            case 7:
                                this->opcodes[i] = &CPU::CPX;
                                this->debug_opcodes[i] = "CPX";
                                break;
                        }
                        break;

                    case 1:
                        switch(a) {
                            case 1:
                                this->opcodes[i] = &CPU::BIT;
                                this->debug_opcodes[i] = "CPX";
                                break;
                            case 4:
                                this->opcodes[i] = &CPU::STY;
                                this->debug_opcodes[i] = "STY";
                                break;
                            case 5:
                                this->opcodes[i] = &CPU::LDY;
                                this->debug_opcodes[i] = "LDY";
                                break;
                            case 6:
                                this->opcodes[i] = &CPU::CPY;
                                this->debug_opcodes[i] = "CPY";
                                break;
                            case 7:
                                this->opcodes[i] = &CPU::CPX;
                                this->debug_opcodes[i] = "CPX";
                                break;
                        }
                        break;
                    case 2:
                        this->addrmodes[i] = nullptr;
                        this->debug_addr[i] = "impl";
                        switch(a) {
                            case 0:
                                this->opcodes[i] = &CPU::PHP;
                                this->debug_opcodes[i] = "PHP";
                                break;
                            case 1:
                                this->opcodes[i] = &CPU::PLP;
                                this->debug_opcodes[i] = "PLP";
                                break;
                            case 2:
                                this->opcodes[i] = &CPU::PHA;
                                this->debug_opcodes[i] = "PHA";
                                break;
                            case 3:
                                this->opcodes[i] = &CPU::PLA;
                                this->debug_opcodes[i] = "PLA";
                                break;
                            case 4:
                                this->opcodes[i] = &CPU::DEY;
                                this->debug_opcodes[i] = "DEY";
                                break;
                            case 5:
                                this->opcodes[i] = &CPU::TAY;
                                this->debug_opcodes[i] = "TAY";
                                break;
                            case 6:
                                this->opcodes[i] = &CPU::INY;
                                this->debug_opcodes[i] = "INY";
                                break;
                            case 7:
                                this->opcodes[i] = &CPU::INX;
                                this->debug_opcodes[i] = "INX";
                                break;

                        }
                        break;
                    case 3:
                        switch(a) {
                            case 1:
                                this->opcodes[i] = &CPU::BIT;
                                this->debug_opcodes[i] = "BIT";
                                break;
                            case 2:
                                this->opcodes[i] = &CPU::JMP;
                                this->debug_opcodes[i] = "JMP";
                                break;
                            case 3:
                                this->opcodes[i] = &CPU::JMP;
                                this->addrmodes[i] = &CPU::ind;
                                this->debug_opcodes[i] = "JMP";
                                this->debug_addr[i] = "ind";
                                break;
                            case 4:
                                this->opcodes[i] = &CPU::STY;
                                this->debug_opcodes[i] = "STY";
                                break;
                            case 5:
                                this->opcodes[i] = &CPU::LDY;
                                this->debug_opcodes[i] = "LDY";
                                break;
                            case 6:
                                this->opcodes[i] = &CPU::CPY;
                                this->debug_opcodes[i] = "CPY";
                                break;
                            case 7:
                                this->opcodes[i] = &CPU::CPX;
                                this->debug_opcodes[i] = "CPX";
                                break;
                        }
                        break;
                    case 4:
                        this->addrmodes[i] = &CPU::rel;
                        this->debug_addr[i] = "rel";
                        switch(a) {
                            case 0:
                                this->opcodes[i] = &CPU::BPL;
                                this->debug_opcodes[i] = "BPL";
                                break;
                            case 1:
                                this->opcodes[i] = &CPU::BMI;
                                this->debug_opcodes[i] = "BMI";
                                break;
                            case 2:
                                this->opcodes[i] = &CPU::BVC;
                                this->debug_opcodes[i] = "BVC";
                                break;
                            case 3:
                                this->opcodes[i] = &CPU::BVS;
                                this->debug_opcodes[i] = "BVS";
                                break;
                            case 4:
                                this->opcodes[i] = &CPU::BCC;
                                this->debug_opcodes[i] = "BCC";
                                break;
                            case 5:
                                this->opcodes[i] = &CPU::BCS;
                                this->debug_opcodes[i] = "BCS";
                                break;
                            case 6:
                                this->opcodes[i] = &CPU::BNE;
                                this->debug_opcodes[i] = "BNE";
                                break;
                            case 7:
                                this->opcodes[i] = &CPU::BEQ;
                                this->debug_opcodes[i] = "BEQ";
                                break;
                        }
                        break;
                    case 5:
                        switch(a) {
                            case 4:
                                this->opcodes[i] = &CPU::STY;
                                this->debug_opcodes[i] = "STY";
                                break;
                            case 5:
                                this->opcodes[i] = &CPU::LDY;
                                this->debug_opcodes[i] = "LDY";
                                break;
                        }
                        break;
                    case 6:
                        this->addrmodes[i] = nullptr;
                        this->debug_addr[i] = "impl";
                        switch(a) {
                            case 0:
                                this->opcodes[i] = &CPU::CLC;
                                this->debug_opcodes[i] = "CLC";
                                break;
                            case 1:
                                this->opcodes[i] = &CPU::SEC;
                                this->debug_opcodes[i] = "SEC";
                                break;
                            case 2:
                                this->opcodes[i] = &CPU::CLI;
                                this->debug_opcodes[i] = "CLI";
                                break;
                            case 3:
                                this->opcodes[i] = &CPU::SEI;
                                this->debug_opcodes[i] = "SEI";
                                break;
                            case 4:
                                this->opcodes[i] = &CPU::TYA;
                                this->debug_opcodes[i] = "TYA";
                                break;
                            case 5:
                                this->opcodes[i] = &CPU::CLV;
                                this->debug_opcodes[i] = "CLV";
                                break;
                            case 6:
                                this->opcodes[i] = &CPU::CLD;
                                this->debug_opcodes[i] = "CLD";
                                break;
                            case 7:
                                this->opcodes[i] = &CPU::SED;
                                this->debug_opcodes[i] = "SED";
                                break;
                        }
                        break;
                    case 7:
                        if (a==5) {
                            this->opcodes[i] = &CPU::LDY;
                            this->debug_opcodes[i] = "LDY";
                        }
                        break;
                }
                break;
            case 1:
                switch(a) {
                    case 0:
                        this->opcodes[i] = &CPU::ORA;
                        this->debug_opcodes[i] = "ORA";
                        break;
                    case 1:
                        this->opcodes[i] = &CPU::AND;
                        this->debug_opcodes[i] = "AND";
                        break;
                    case 2:
                        this->opcodes[i] = &CPU::EOR;
                        this->debug_opcodes[i] = "EOR";
                        break;
                    case 3:
                        this->opcodes[i] = &CPU::ADC;
                        this->debug_opcodes[i] = "ADC";
                        break;
                    case 4:
                        this->opcodes[i] = &CPU::STA;
                        this->debug_opcodes[i] = "STA";
                        break;
                    case 5:
                        this->opcodes[i] = &CPU::LDA;
                        this->debug_opcodes[i] = "LDA";
                        break;
                    case 6:
                        this->opcodes[i] = &CPU::CMP;
                        this->debug_opcodes[i] = "CMP";
                        break;
                    case 7:
                        this->opcodes[i] = &CPU::SBC;
                        this->debug_opcodes[i] = "SBC";
                        break;
                }
                switch(b) {
                    case 0:
                        this->addrmodes[i] = &CPU::xind;
                        this->debug_addr[i] = "xind";
                        break;
                    case 2:
                        this->addrmodes[i] = &CPU::imm;
                        this->debug_addr[i] = "imm";
                        break;
                    case 4:
                        this->addrmodes[i] = &CPU::indy;
                        this->debug_addr[i] = "indy";
                        break;
                    case 6:
                        this->addrmodes[i] = &CPU::absy;
                        this->debug_addr[i] = "absy";
                        break;
                }
                break;
            case 2:
                switch(a) {
                    case 0:
                        this->opcodes[i] = &CPU::ASL;
                        this->debug_opcodes[i] = "ASL";
                        break;
                    case 1:
                        this->opcodes[i] = &CPU::ROL;
                        this->debug_opcodes[i] = "ROL";
                        break;
                    case 2:
                        this->opcodes[i] = &CPU::LSR;
                        this->debug_opcodes[i] = "LSR";
                        break;
                    case 3:
                        this->opcodes[i] = &CPU::ROR;
                        this->debug_opcodes[i] = "ROR";
                        break;
                    case 4:
                        if (b==2) {
                            this->opcodes[i] = &CPU::TXA;
                            this->debug_opcodes[i] = "TXA";
                        } else if (b==6) {
                            this->opcodes[i] = &CPU::TXS;
                            this->debug_opcodes[i] = "TXS";
                        } else {
                            this->opcodes[i] = &CPU::STX;
                            this->debug_opcodes[i] = "STX";
                        }
                        break;
                    case 5:
                        if (b==2) {
                            this->opcodes[i] = &CPU::TAX;
                            this->debug_opcodes[i] = "TAX";
                        } else if (b==6) {
                            this->opcodes[i] = &CPU::TSX;
                            this->debug_opcodes[i] = "TSX";
                        } else {
                            this->opcodes[i] = &CPU::LDX;
                            this->debug_opcodes[i] = "LDX";
                        }
                        break;
                    case 6:
                        if (b==2) {
                            this->opcodes[i] = &CPU::DEX;
                            this->debug_opcodes[i] = "DEX";
                        } else {
                            this->opcodes[i] = &CPU::DEC;
                            this->debug_opcodes[i] = "DEC";
                        }
                        break;
                    case 7:
                        if (b==2) {
                            this->opcodes[i] = &CPU::NOP;
                            this->debug_opcodes[i] = "NOP";
                        } else {
                            this->opcodes[i] = &CPU::INC;
                            this->debug_opcodes[i] = "INC";
                        }
                        break;
                }
                switch(b) {
                    case 0:
                        this->addrmodes[i] = &CPU::imm;
                        this->debug_addr[i] = "imm";
                        break;
                    case 2:
                        if (a<4) {
                            this->addrmodes[i] = &CPU::acc;
                            this->debug_addr[i] = "acc";
                        } else {
                            this->addrmodes[i] = nullptr;
                            this->debug_addr[i] = "impl";
                        }
                        break;
                    case 5:
                        if (a==4||a==5) {
                            this->addrmodes[i] = &CPU::zpgy;
                            this->debug_addr[i] = "zpgy";
                        }
                        break;
                    case 6:
                        this->addrmodes[i] = nullptr;
                        this->debug_addr[i] = "impl";
                        break;
                    case 7:
                        if (a==5) {
                            this->addrmodes[i] = &CPU::absy;
                            this->debug_addr[i] = "absy";
                        }
                        break;
                }
                break;
        }   
        
    }
}