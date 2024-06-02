#ifndef cpu_h
#define cpu_h

#include "rom.h"
#include "apu.h"
#include "glob_const.h"
#include "controller.h"
#include <cstdint>


namespace NES {
    
class PPU;

class APU;

class Controller;

class CPU {
    public:
        CPU();
        CPU(bool dbug);
        PPU* ppu;
        APU* apu;
        //int CLOCK_SPEED = 1790210;
        int CLOCK_SPEED = 1789773; //original
        //int CLOCK_SPEED = 1800000; //slightly clocked up so that audio doesnt underflow
        //int CLOCK_SPEED = 2147483647; //test how fast it can go
        int emulated_clock_speed(long long elapsed);
        long long start = epoch();
        void clock();
        //int breakpoints[1] = {0xe34e}; //for testing
        //int breakpoints[1] = {0xe3bd};
        int breakpoints[1] = {0xa0d3};
        int8_t accumulator;
        int8_t x;
        int8_t y;
        int8_t inputs[2] = {0}; //inputs for each joystick
        int8_t* pc;
        uint8_t ins_size;
        bool get_flag(char flag);
        void set_flag(char flag,bool val);
        const uint16_t NMI = 0xFFFA;
        const uint16_t RESET = 0xFFFC;
        const uint16_t IRQ = 0xFFFE;
        void reset();
        typedef int8_t* (CPU::*addressing_mode) (int8_t*);
        typedef void (CPU::*instruction) (int8_t*);
        addressing_mode addrmodes[256];
        instruction opcodes[256];
        void ins_str(char * write,uint8_t opcode);
        void ins_str_mem(char * write,uint8_t* mem,int8_t* arg_ptr);
        void loadRom(ROM *r,bool ram = true);
        void init_vals() {
            for (int i=0; i<=0xffff; i++) {
                memory[i] = 0;
            }
            flags = 0x24;
            accumulator = 0;
            x = 0;
            y = 0;
            sp = 0xff;
        }
        long long ins_num = 0;
        int8_t memory[0x10000] ={0};
        ROM* rom;
        void write(int8_t* address,int8_t value);
        int8_t read(int8_t* address, bool from_cpu = true);
        long long cycles = 0;
        int current_cycles = 0;
        bool read_nmi = false;
        void start_nmi();
        void start_irq();
        bool recv_nmi = false;
        bool nmi_next = false;
        bool recv_irq = false;
        bool nmi_output = false;
        bool last_nmi = false;
        uint8_t status() { return flags;}
        bool input_strobe = 0;
        bool debug = false;
        int prg_bank_num = 0;
        long long last = epoch_nano();
        long long elapsed_time = 0;
        void save_state(FILE* data);
        void load_state(FILE* data);
        void save_ram(FILE* data);
        void load_ram(FILE* data);
        void set_controller(Controller* cont,uint8_t port);
        Controller* conts[2];
    private:

        //---- instructions ----
        void define_timings();
        void define_opcodes();
        void ADC(int8_t* args);
        void AND(int8_t* args);
        void ASL(int8_t* args);
        void BIT(int8_t* args);
        void BRK(int8_t* args);
        void CMP(int8_t* args);
        void CPX(int8_t* args);
        void CPY(int8_t* args);
        void DEC(int8_t* args);
        void EOR(int8_t* args);
        void INC(int8_t* args);
        void JMP(int8_t* args);
        void JSR(int8_t* args);
        void LDA(int8_t* args);
        void LDX(int8_t* args);
        void LDY(int8_t* args);
        void LSR(int8_t* args);
        void NOP(int8_t* args);
        void ORA(int8_t* args);
        void ROL(int8_t* args);
        void ROR(int8_t* args);
        void RTI(int8_t* args);
        void RTS(int8_t* args);
        void SBC(int8_t* args);
        void STA(int8_t* args);
        void STX(int8_t* args);
        void STY(int8_t* args);
        void TAX(int8_t* args);
        void TXA(int8_t* args);
        void TSX(int8_t* args);
        void TXS(int8_t* args);
        void DEX(int8_t* args);
        void INX(int8_t* args);
        void TAY(int8_t* args);
        void TYA(int8_t* args);
        void DEY(int8_t* args);
        void INY(int8_t* args);
        void CLC(int8_t* args);
        void SEC(int8_t* args);
        void CLI(int8_t* args);
        void SEI(int8_t* args);
        void CLV(int8_t* args);
        void CLD(int8_t* args);
        void SED(int8_t* args);
        void PHP(int8_t* args);
        void BPL(int8_t* args);
        void PLP(int8_t* args);
        void PHA(int8_t* args);
        void PLA(int8_t* args);
        void BMI(int8_t* args);
        void BVC(int8_t* args);
        void BVS(int8_t* args);
        void BCC(int8_t* args);
        void BCS(int8_t* args);
        void BNE(int8_t* args);
        void BEQ(int8_t* args);

        //----addressing modes----

        int8_t* acc(int8_t* args);
        int8_t* xind(int8_t* args);
        int8_t* indy(int8_t* args);
        int8_t* zpg(int8_t* args);
        int8_t* zpgx(int8_t* args);
        int8_t* zpgy(int8_t* args);
        int8_t* abs(int8_t* args);
        int8_t* absx(int8_t* args);
        int8_t* absy(int8_t* args);
        int8_t* ind(int8_t* args);
        int8_t* rel(int8_t* args);
        int8_t* imm(int8_t* args) {ins_size = 2; map_memory(&args); return &args[0];}

        //--extras--
        uint8_t sp = 0xff;
        uint8_t flags = 0x24; // bits: NV-BDIZC
        long long get_addr(int8_t* ptr);
        void stack_push(int8_t val);
        uint8_t stack_pull(void);
        void map_memory(int8_t** address); //designate mirrors and important registers, and anything necessary for bank switching and the like according to the set mapper number.
        char* debug_opcodes[256] = {0};
        uint8_t inst_cycles[256] = {0};
        uint8_t inst_cycles_pagecross[256] = {0};
        char* debug_addr[256] = {0};
};

}

namespace SNES {

class CPU {
    public:
        uint16_t X;
        uint16_t Y;
        uint16_t A;
        uint16_t S;
        uint32_t PC;
        
        int clock_speeds[3] = {1790000,2680000,3580000};

};

}
#endif