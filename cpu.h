#include <stdint.h>

class CPU {
    public:
        int CLOCK_SPEED = 1789773;
        void clock(uint8_t* ins);
        uint8_t accumulator;
        uint8_t x;
        uint8_t y;
        uint8_t* pc;
        uint8_t flags = 0x20; // bits: NV1BDIZC
        const uint16_t NMI = 0xFFFA;
        const uint16_t RESET = 0xFFFC;
        const uint16_t IRQ = 0xFFFE;
        void reset();
        typedef uint8_t* (CPU::*addressing_mode) (uint8_t*);
        typedef void (CPU::*instruction) (uint8_t*);
        addressing_mode addrmodes[256];
        instruction opcodes[256];
    private:

        //---- instructions ----
        void define_opcodes();
        void ADC(uint8_t* args);
        void AND(uint8_t* args);
        void ASL(uint8_t* args);
        void BIT(uint8_t* args);
        void BRK(uint8_t* args);
        void CMP(uint8_t* args);
        void CPX(uint8_t* args);
        void CPY(uint8_t* args);
        void DEC(uint8_t* args);
        void EOR(uint8_t* args);
        void INC(uint8_t* args);
        void JMP(uint8_t* args);
        void JSR(uint8_t* args);
        void LDA(uint8_t* args);
        void LDX(uint8_t* args);
        void LDY(uint8_t* args);
        void LSR(uint8_t* args);
        void NOP(uint8_t* args);
        void ORA(uint8_t* args);
        void ROL(uint8_t* args);
        void ROR(uint8_t* args);
        void RTI(uint8_t* args);
        void RTS(uint8_t* args);
        void SBC(uint8_t* args);
        void STA(uint8_t* args);
        void STX(uint8_t* args);
        void STY(uint8_t* args);
        void TAX(uint8_t* args);
        void TXA(uint8_t* args);
        void TSX(uint8_t* args);
        void TXS(uint8_t* args);
        void DEX(uint8_t* args);
        void INX(uint8_t* args);
        void TAY(uint8_t* args);
        void TYA(uint8_t* args);
        void DEY(uint8_t* args);
        void INY(uint8_t* args);
        void CLC(uint8_t* args);
        void SEC(uint8_t* args);
        void CLI(uint8_t* args);
        void SEI(uint8_t* args);
        void CLV(uint8_t* args);
        void CLD(uint8_t* args);
        void SED(uint8_t* args);
        void PHP(uint8_t* args);
        void BPL(uint8_t* args);
        void PLP(uint8_t* args);
        void PLP(uint8_t* args);
        void PHA(uint8_t* args);
        void PLA(uint8_t* args);

        //----addressing modes----

        uint8_t* xind(uint8_t* args);
        uint8_t* indy(uint8_t* args);
        uint8_t* zpg(uint8_t* args);
        uint8_t* zpgx(uint8_t* args);
        uint8_t* zpgy(uint8_t* args);
        uint8_t* abs(uint8_t* args);
        uint8_t* absx(uint8_t* args);
        uint8_t* absy(uint8_t* args);
        uint8_t* ind(uint8_t* args);
        uint8_t* rel(uint8_t* args);
        uint8_t* imm(uint8_t* args) {return &args[0];}
        uint16_t get_addr(uint8_t* ptr);
        uint8_t memory[0xFFFF];
};