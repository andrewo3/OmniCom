#ifndef SNES_CPU_H
#define SNES_CPU_H
#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include "rom.h"

namespace SNES {

class ROM;
class CPU {
    public:
        //Constructor
        CPU();
        //Registers
        uint8_t K = 0;
        uint16_t S = 0;
        uint8_t X = 0;
        uint8_t Y = 0;
        uint16_t PC;
        //Flags
        /*
        P bit 7: n flag
        P bit 6: v flag
        P bit 5: m flag (native mode)
        P bit 4: x flag (native mode), b flag (emulation mode)
        P bit 3: d flag
        P bit 2: i flag
        P bit 1: z flag
        P bit 0: c flag*/
        uint8_t P;
        bool e; //emulation flag
        //misc
        uint8_t memory[0x10000];
        bool new_func;
        typedef void (*func_branch)(uint8_t&, bool&); //pass flags into function
        typedef uint8_t* (*addrmode)(uint8_t*);
        std::map<uint32_t,func_branch> func_cache; 
        ROM* rom;
        void clock();
        void reset();
        uint8_t read(uint32_t addr);
    private:
        uint8_t* abs(uint8_t* arg);
        uint8_t* absx(uint8_t* arg);
        uint8_t* absy(uint8_t* arg);
        bool get_flag(char flag);
        void set_flag(char flag,bool set);
        void SEI(std::vector<uint8_t>& wr);
        void CLC(std::vector<uint8_t>& wr);
        void PHK(std::vector<uint8_t>& wr);
        void XCE(std::vector<uint8_t>& wr);
        void build_function(std::vector<uint8_t>& wr);
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