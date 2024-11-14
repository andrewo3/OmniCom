#ifndef SNES_CPU_H
#define SNES_CPU_H
#include <cstdint>
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
        //vectors
        //vec[1] = with emulation mode
        //vec[0] = without
        ROM* rom;
        void clock();
        void reset();
        uint8_t read(uint32_t addr);

};

}

#endif