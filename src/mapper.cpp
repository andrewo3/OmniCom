#include "mapper.h"
#include "cpu.h"
#include "ppu.h"


void MMC3::map_write(void** ptrs, int8_t* address, int8_t *value) {
    int8_t val = *value;
    CPU* cpu = (CPU*)ptrs[0];
    ROM* rom = cpu->rom;
    PPU* ppu = (PPU*)ptrs[1];
    long long location = address-(cpu->memory);
    if (0x8000<=location && location<=0x9fff && !(location&0x1)) { //bank select
        reg = val;
        //if new $8000.D6 is different from last value, swap $8000 and $C000
        //printf("CHANGE: R%i\n",reg&0x7);
        if ((val&0x40)!=(xbase&0x40)) {
            int8_t temp[0x2000];
            //printf("R6 at 0: %s\n",val&0x40 ? "false": "true");
            memcpy(temp,&cpu->memory[0x8000],0x2000); //copy $8000 to temp
            memcpy(&cpu->memory[0x8000],&cpu->memory[0xC000],0x2000); //copy (-2) to R6
            memcpy(&cpu->memory[0xC000],temp,0x2000); //copy temp to (old) (-2) 
        }
        //do same for ppu memory and $8000.D7
        if ((val&0x80)!=(xbase&0x80)) {
            int8_t temp[0x1000];
            memcpy(temp,ppu->memory,0x1000); //copy $0000 to temp
            memcpy(ppu->memory,&ppu->memory[0x1000],0x1000); //copy 0x1000 to 0x0000
            memcpy(&ppu->memory[0x1000],temp,0x1000); //copy temp to 0x1000 
        }
        xbase = val;
    } else if (0x8000<=location && location<=0x9fff && (location&0x1)) { //bank data
        uint8_t r = reg&0x7;
        int chrsize = (rom->get_chrsize())/0x2000;
        int prgsize = (rom->get_prgsize())/0x4000;
        //printf("R%i IS BANK NUM: %i\n",r,val);
        if (r<6) {
            uint16_t start_loc;
            if (!(xbase&0x80)) {
                    if (r<2) {
                        start_loc = 0x800*r;
                    } else {
                        start_loc = 0x1000+0x400*(r-2);
                    }
            } else {
                if (r<2) {
                    start_loc = 0x1000+0x800*r;
                } else {
                    start_loc = 0x400*(r-2);
                }
            }
            int bank_size = (0x400<<(r<2));
            memcpy(ppu->memory+start_loc,rom->get_chr_bank((val&(~(r<2)))),bank_size);
        } else {
            uint16_t start_loc = 0x2000*(r==7)+0x4000*(r!=7 && (xbase&0x40));
            memcpy(&cpu->memory[0x8000]+start_loc,rom->get_prg_bank((val&0x3F)<<3),0x2000);
        }
    } else if (0xA000<=location && location<=0xBFFF && !(location&0x1) && rom->mirrormode!=FOURSCREEN) { //mirroring
        rom->mirrormode = (NT_MIRROR)!(val&0x1); //0 is vertical, 1 is horizontal - opposite of the enum defined in rom.h
    } else if (0xA000<=location && location<=0xBFFF && (location&0x1)) { //prg ram protect
        wp = val&0x40;
        prgram = val&0x80; //honestly dont know what to do with this flag
    } else if (0xC000<=location && location <=0xDFFF && !(location&0x1)) { // IRQ latch
        irq_reload = (uint8_t)val;
        //printf("New Reload Value: %i\n",irq_reload);
    } else if (0xC000<=location && location <=0xDFFF && (location&0x1)) { // IRQ reload
        irq_counter = -1; // on next clock this will immediately trigger reload (without triggering irq)
    } else if (0xE000<=location && location <=0xFFFF && !(location&0x1)) { // IRQ disable
        //printf("Disable IRQ MMC3\n");
        irq_enabled = false;
    } else if (0xE000<=location && location <=0xFFFF && (location&0x1)) { // IRQ enable
        //printf("Enable IRQ MMC3\n");
        irq_enabled = true;
    }
    if (location==0x2006 && ppu->w==0 && ppu->address_bus&0x1000 && !(last_v&0x1000)) { //PPUADDR write A12 on after previously being off
        //printf("PPUADDR: %04x prev: %04x\n",ppu->v,last_v);
        //scanline_clock(cpu);
    }

    //write protect
    if (wp && 0x6000<=location && location<=0x7FFF) {
        *value = *address; //set the value to the number already at the address, so when its written - nothing changes
    }

}

void MMC3::scanline_clock(CPU* cpu) {
    //printf("SCANLINE CLOCK\n");
    irq_counter--;
    //printf("PPU ADDRESS ON CLOCK: %04x\n",ppu->v);
    if (irq_counter == 0 && irq_enabled) { 
        cpu->recv_irq = true;
    }
    if (irq_counter<=0) {
        irq_counter = irq_reload;
    }
}

void MMC3::clock(void** system) {
    CPU* cpu = (CPU*)system[0];
    ROM* rom = cpu->rom;
    PPU* ppu = (PPU*)system[1];
    bool rendering = ((*(ppu->PPUMASK))&0x18);
    if ((ppu->address_bus&0x1000) && !(last_v&0x1000)) { //rising edge of a12
        if (off_clocks>=9) {
            scanline_clock(cpu);
            //printf("Scanline Counter: %i on scanline %i - reload value: %i\n",irq_counter,ppu->scanline,irq_reload);
        }
        off_clocks = 0;
    }
    last_v = ppu->address_bus;
    if ((last_v&0x1000)==0) {
        off_clocks++;
    }
}

void CNROM::map_write(void** ptrs, int8_t* address, int8_t *value) {
    int8_t val = *value;
    CPU* cpu = (CPU*)ptrs[0];
    PPU* ppu = (PPU*)ptrs[1];
    long long location = address-(cpu->memory);
    if (0x8000<=location && location<=0xffff) {
        int chrsize = (ppu->rom->get_chrsize())/0x2000;
        ppu->chr_bank_num = ((uint8_t)val%chrsize)<<3;
        //printf("CHANGE! %i\n",ppu->chr_bank_num);
        ppu->loadRom(ppu->rom);
    }   
}

void UxROM::map_write(void** ptrs, int8_t* address, int8_t* value) {
    int8_t val = *value;
    CPU* cpu = (CPU*)ptrs[0];
    long long location = address-(cpu->memory);
    if (0x8000<=location && location<=0xffff) {
        cpu->prg_bank_num = (val&0xf)<<4;
        //printf("CHANGE! %i\n",ppu->chr_bank_num);
        cpu->loadRom(cpu->rom);
    }   
}