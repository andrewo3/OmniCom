#ifndef SNES_CPU_H
#define SNES_CPU_H

namespace SNES {

class CPU {
    public:
        //Registers
        uint16_t X; //X reg
        uint16_t Y; //Y reg
        uint16_t A; //Accumulator
        uint16_t S; //Stack Reg
        uint16_t PC; //Program Counter
        uint8_t DBR; //Data Bank Register
        uint8_t K; //Program Bank Register
        uint16_t D; //Direct Register

        //Flags
        bool B; //break flag
        bool C; //carry flag
        bool D; //Decimal flag
        bool E; //Emulation Flag
        bool I; //Interrupt flag
        bool M; //Accumulator & Mem width flag
        bool N; //Negative flag
        bool V; //Overflow flag
        bool X; //index reg width flag
        bool Z; //zero flag 
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