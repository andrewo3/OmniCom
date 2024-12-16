#ifndef SNES_CPU_H
#define SNES_CPU_H
#include <cstdint>
#include <map>
#include <string>
#include "rom.h"

namespace SNES {

class ROM;
class CPU {
    public:
        //Constructor
        CPU();
        //Registers

        //Flags
        uint8_t bank = 0;
        uint8_t memory[0x10000];
        uint32_t PC;
        uint16_t SP;
        //vectors
        //vec[1] = with emulation mode
        //vec[0] = without
        bool new_func;
        typedef void (*func_branch)();
        typedef uint8_t* (*addrmode)(uint8_t*);
        std::map<uint32_t,func_branch> func_cache; 
        ROM* rom;
        void clock();
        void reset();
        uint8_t read(uint32_t addr);
    private:
        uint8_t* PC_rel_long(uint8_t* arg);
        uint8_t* stack_rel(uint8_t* arg);
        uint8_t* implied(uint8_t* arg);
};


class Instruction {
    public:
        std::string name; //name of instruction as string
        uint8_t opcode; //byte opcode
        uint8_t arg_bytes; //number of bytes it takes for arguments
        virtual void translate(uint8_t* addr) = 0; //translate this instruction into its native assembly counterpart and store it at pointer "addr"
    private:
};

}

#endif