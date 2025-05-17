#include "cpu.h"
#include <string>
#include <vector>
#if defined(__x86_64__) || (_M_X64)
        
#elif defined(__aarch64__) || defined(_M_ARM64)
    #include "ARM.h"
    #define ARG_P 9
    #define ARG_E 10
#endif

//Good reference: http://www.6502.org/tutorials/65c816opcodes.html

using namespace SNES;

uint8_t* CPU::abs(uint8_t* arg) {
    return (uint8_t*)(memory+((this->K<<16)|(arg[1]<<8)|arg[0]));
}

uint8_t* CPU::absx(uint8_t* arg) {
    if (!get_flag('E') && !get_flag('M')) { //24-bit
        return (uint8_t*)(memory+((this->K<<16)|(arg[1]<<8)|arg[0]) + this->X);
    } else if (get_flag('E')) { //16-bit
        return (uint8_t*)(memory+(uint16_t)((arg[1]<<8)|arg[0] + this->X));
    } else {
        return nullptr;
    }
}

uint8_t* CPU::absy(uint8_t* arg) {
    if (!get_flag('E') && !get_flag('M')) { //24-bit
        return (uint8_t*)(memory+((this->K<<16)|(arg[1]<<8)|arg[0]) + this->Y);
    } else if (get_flag('E')) { //16-bit
        return (uint8_t*)(memory+(uint16_t)((arg[1]<<8)|arg[0] + this->Y));
    } else {
        return nullptr;
    }
}

void CPU::CLC(std::vector<uint8_t>& wr) {
    #if defined(__x86_64__) || (_M_X64)
        
    #elif defined(__aarch64__) || defined(_M_ARM64)
        //emulated register P is placed in register 0.
        ARM::AND(wr,9,9,6,-1); // AND to set C flag off in register P
    #endif
}

void CPU::SEI(std::vector<uint8_t>& wr) {
    #if defined(__x86_64__) || (_M_X64)
        
    #elif defined(__aarch64__) || defined(_M_ARM64)
        //emulated register P is placed in register 0.
        ARM::ORR(wr,ARG_P,ARG_P,0,-2); // ORR with register P to set flag on.
    #endif
}


void CPU::XCE(std::vector<uint8_t>& wr) {
    #if defined(__x86_64__) || (_M_X64)
    
    #elif defined(__aarch64__) || defined(_M_ARM64)
        ARM::EOR_reg(wr,15,ARG_E,ARG_P); //get XOR of C flag and E flag
        ARM::AND(wr,15,15,0,0); // Isolate C xor E
        ARM::EOR_reg(wr,ARG_P,15,ARG_P);
        ARM::EOR_reg(wr,ARG_E,15,ARG_E);
    #endif
}

void CPU::build_function(std::vector<uint8_t>& wr) {
    printf("get first val: %02x\n",read(rom->map(PC)));
    uint32_t cycles = 0;
    bool branch_reached = false;

    //setup registers in function
    #if defined(__aarch64__) || defined(_M_ARM64)
        ARM::LDR(wr,ARG_P,0,0); //put P register in register 9
        ARM::LDR(wr,ARG_E,1,0); //take arg 1 (e flag) and place in reg 10
    #elif defined(__x86_64__) || (_M_X64)

    #endif
    while (!branch_reached) {
        uint8_t val = read(rom->map(PC));
        switch (val) {
            case 0x18:
                CPU::CLC(wr);
                cycles+=2;
                break;
            case 0x4b:
                CPU::PHK(wr);
                cycles+=3;
                break;
            case 0x78:
                CPU::SEI(wr);
                cycles+=2;
                break;
            case 0xfb:
                CPU::XCE(wr);
                cycles+=2;
                break;
            default:
                printf("Invalid Instruction: %02x\n",val);
                exit(1);
        }
        PC++;
    }
    //write flags back in and return from function
    #if defined(__aarch64__) || defined(_M_ARM64)
        ARM::STR(wr,ARG_P,0,0);
        ARM::STR(wr,ARG_E,1,0);
        ARM::RET(wr);
    #elif defined(__x86_64__) || (_M_X64)

    #endif
}