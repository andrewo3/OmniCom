#include "mapper.h"
#include "cpu.h"
#include "ppu.h"


void MMC3::map_write(void** ptrs, int8_t* address, int8_t val) {
    CPU* cpu = (CPU*)ptrs[0];
    long long location = address-(cpu->memory);
    if (0x8000<=location && location<=0x9fff && !(location&0x1)) { //bank select
        reg = val&0x7;
    } else if (0x8000<=location && location<=0x9fff && (location&0x1)) { //bank data
        
    }   
}

void CNROM::map_write(void** ptrs, int8_t* address, int8_t val) {
    CPU* cpu = (CPU*)ptrs[0];
    PPU* ppu = (PPU*)ptrs[1];
    long long location = address-(cpu->memory);
    if (0x8000<=location && location<=0xffff) {
        ppu->chr_bank_num = val&0x3;
        printf("CHANGE! %i\n",ppu->chr_bank_num);
        ppu->loadRom(ppu->rom);
    }   
}