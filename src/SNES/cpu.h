#ifndef SNES_CPU_H
#define SNES_CPU_H

namespace SNES {

class CPU {
    public:
        uint16_t X; //X reg
        uint16_t Y; //Y reg
        uint16_t A; //Accumulator
        uint16_t S; //Stack Reg
        uint16_t PC; //Program Counter
        uint8_t DBR; //Data Bank Register
        uint8_t PBR; //Program Bank Register
        uint16_t D; //Direct Register
        bool E; // Emulation Flag
        uint8_t P; //Processor Status
        uint8_t memory[0x20000];
        //vectors
        //vec[1] = with emulation mode
        //vec[0] = without
        uint16_t COP[2] = {0xFFE4,0xFFF4};
        uint16_t BRK[2] = {0xFFE6,0xFFFE};
        uint16_t NMI[2] = {0xFFEA,0xFFFA};
        uint16_t IRQ[2] = {0xFFEE,0xFFFE};
        uint16_t RESET = 0xFFFC;

        int clock_speed[2] = {2680000,3580000};
        void clock();

};

}

#endif