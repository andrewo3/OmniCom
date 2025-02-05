#include <vector>

//ARM opcode emitters
#define ALWAYS 0b1110

namespace ARM {
enum opcodes {
    OPCODE_AND, 
    OPCODE_EOR, 
    OPCODE_SUB,
    OPCODE_RSB,
    OPCODE_ADD,
    OPCODE_ADC,
    OPCODE_SBC,
    OPCODE_RSC,
    OPCODE_TST,
    OPCODE_TEQ,
    OPCODE_CMP,
    OPCODE_CMN,
    OPCODE_ORR,
    OPCODE_MOV,
    OPCODE_BIC,
    OPCODE_MVN};

void MOV(std::vector<uint8_t>& wr) {

}

//move immediate 8-bit int "imm" to register "Rd"
void MOVS_imm(std::vector<uint8_t>& wr,uint8_t Rd,uint8_t imm) {
    uint32_t ins = ALWAYS<<28; //unconditional flag
    ins|=(1<<25); //immediate operand
    ins|=OPCODE_MOV<<21;
    ins|=imm;
    for (int i=0; i<4; i++) {
        wr.push_back(((uint8_t*)&ins)[i]);
    }
}

void LDR(std::vector<uint8_t>& wr, uint8_t Rt, uint8_t Rn, uint16_t offset) {
    uint32_t ins = 0b1011100101<<22;
    ins|=(offset&0xfff)<<10;
    ins|=(Rn&0x1f)<<5;
    ins|=Rt&0x1f;
    for (int i=0; i<4; i++) {
        wr.push_back(((uint8_t*)&ins)[i]);
    }
}
void STR(std::vector<uint8_t>& wr, uint8_t Rt, uint8_t Rn, uint16_t offset) {
    uint32_t ins = 0b10111000000<<21;
    ins|=(offset&0x1ff)<<12;
    ins|=1<<10;
    ins|=(Rn&0x1f)<<5;
    ins|=Rt&0x1f;
    for (int i=0; i<4; i++) {
        wr.push_back(((uint8_t*)&ins)[i]);
    }
}

void ORR(std::vector<uint8_t>& wr,uint8_t Rd, uint8_t Rn, uint8_t imms,uint8_t immr) {
   //https://developer.arm.com/documentation/ddi0602/2024-12/Base-Instructions/ORR--immediate---Bitwise-OR--immediate--?lang=en
    uint32_t ins = 0b0011001000<<22; //unconditional flag
    ins|=(imms&0x3f)<<10; //immediate operand
    ins|=(immr&0x3f)<<16;
    ins|=(Rn&0x1f)<<5;
    ins|=(Rd&0x1f);
    for (int i=0; i<4; i++) {
        wr.push_back(((uint8_t*)&ins)[i]);
    }
}

void AND(std::vector<uint8_t>& wr,uint8_t Rd, uint8_t Rn, uint8_t imms,uint8_t immr) {
   //https://developer.arm.com/documentation/ddi0602/2024-12/Base-Instructions/ORR--immediate---Bitwise-OR--immediate--?lang=en
    uint32_t ins = 0b0001001000<<22; //unconditional flag
    ins|=(imms&0x3f)<<10; //immediate operand
    ins|=(immr&0x3f)<<16;
    ins|=(Rn&0x1f)<<5;
    ins|=(Rd&0x1f);
    for (int i=0; i<4; i++) {
        wr.push_back(((uint8_t*)&ins)[i]);
    }
}

void RET(std::vector<uint8_t>& wr) {
    uint32_t ins = 0b11010110010111110000001111000000;
    for (int i=0; i<4; i++) {
        wr.push_back(((uint8_t*)&ins)[i]);
    }
}

}