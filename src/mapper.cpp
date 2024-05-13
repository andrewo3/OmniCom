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
            memcpy(ppu->memory+start_loc,rom->get_chr_bank((uint8_t)(val&(~(r<2)))),bank_size);
        } else {
            uint16_t start_loc = 0x2000*(r==7)+0x4000*(r!=7 && (xbase&0x40));
            memcpy(&cpu->memory[0x8000]+start_loc,rom->get_prg_bank((uint16_t)((val&0x3F)<<3)),0x2000);
        }
    } else if (0xA000<=location && location<=0xBFFF && !(location&0x1) && ppu->mirrormode!=FOURSCREEN) { //mirroring
        ppu->mirrormode = (NT_MIRROR)!(val&0x1); //0 is vertical, 1 is horizontal - opposite of the enum defined in rom.h
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
    //(ppu->address_bus&0x1000) && !(last_v&0x1000)
    if (ppu->scycle==256 && rendering && ppu->vblank==false) { //rising edge of a12
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

void MMC3::serialize(void** system, char* out) {
    int ind = 0;
    memcpy(out+ind,&reg,sizeof(uint8_t));
    ind+=sizeof(uint8_t);
    memcpy(out+ind,&xbase,sizeof(uint8_t));
    ind+=sizeof(uint8_t);
    memcpy(out+ind,&wp,sizeof(bool));
    ind+=sizeof(bool);
    memcpy(out+ind,&prgram,sizeof(bool));
    ind+=sizeof(bool);
    memcpy(out+ind,&irq_enabled,sizeof(bool));
    ind+=sizeof(bool);
    memcpy(out+ind,&last_v,sizeof(uint16_t));
    ind+=sizeof(uint16_t);
    memcpy(out+ind,&irq_counter,sizeof(int));
    ind+=sizeof(int);
    memcpy(out+ind,&irq_reload,sizeof(uint8_t));
    ind+=sizeof(uint8_t);
    memcpy(out+ind,&off_clocks,sizeof(uint8_t));
    ind+=sizeof(uint8_t);
    memcpy(out+ind,&scanline_counted,sizeof(bool));
}

void MMC3::deserialize(void** system, char* in) {
    int ind = 0;
    memcpy(&reg,in+ind,sizeof(uint8_t));
    ind+=sizeof(uint8_t);
    memcpy(&xbase,in+ind,sizeof(uint8_t));
    ind+=sizeof(uint8_t);
    memcpy(&wp,in+ind,sizeof(bool));
    ind+=sizeof(bool);
    memcpy(&prgram,in+ind,sizeof(bool));
    ind+=sizeof(bool);
    memcpy(&irq_enabled,in+ind,sizeof(bool));
    ind+=sizeof(bool);
    memcpy(&last_v,in+ind,sizeof(uint16_t));
    ind+=sizeof(uint16_t);
    memcpy(&irq_counter,in+ind,sizeof(int));
    ind+=sizeof(ind);
    memcpy(&irq_reload,in+ind,sizeof(uint8_t));
    ind+=sizeof(uint8_t);
    memcpy(&off_clocks,in+ind,sizeof(uint8_t));
    ind+=sizeof(uint8_t);
    memcpy(&scanline_counted,in+ind,sizeof(bool));
}

void CNROM::map_write(void** ptrs, int8_t* address, int8_t *value) {
    int8_t val = *value;
    CPU* cpu = (CPU*)ptrs[0];
    PPU* ppu = (PPU*)ptrs[1];
    long long location = address-(cpu->memory);
    if (0x8000<=location && location<=0xffff) {
        int chrsize = (ppu->rom->get_chrsize())/0x2000;
        ppu->chr_bank_num = ((uint8_t)val%chrsize)<<3;
        bank_num = ppu->chr_bank_num;
        //printf("CHANGE! %i\n",ppu->chr_bank_num);
        ppu->loadRom(ppu->rom);
    }   
}

void CNROM::deserialize(void** system, char* in) {
    PPU* ppu = (PPU*)system[1];
    memcpy(&bank_num,in,sizeof(int));
    ppu->chr_bank_num = bank_num;
    ppu->loadRom(ppu->rom);
}

void UxROM::deserialize(void** system, char* in) {
    CPU* cpu = (CPU*)system[0];
    memcpy(&bank_num,in,sizeof(int));
    cpu->prg_bank_num = bank_num;
    cpu->loadRom(cpu->rom);
}

void UxROM::map_write(void** ptrs, int8_t* address, int8_t* value) {
    int8_t val = *value;
    CPU* cpu = (CPU*)ptrs[0];
    long long location = address-(cpu->memory);
    if (0x8000<=location && location<=0xffff) {
        cpu->prg_bank_num = (val&0xf)<<4;
        bank_num = cpu->prg_bank_num;
        //printf("CHANGE! %i\n",ppu->chr_bank_num);
        cpu->loadRom(cpu->rom);
    }   
}

void NTDEC2722::map_write(void** ptrs, int8_t* address, int8_t* value) {
    CPU* cpu = (CPU*)ptrs[0];
    ROM* rom = cpu->rom;
    PPU* ppu = (PPU*)ptrs[1];
    long long location = address-(cpu->memory);
    if (0x8000<=location && location <=0x9fff) {
        enabled = false;
        counter = 4096*3;
    } else if (0xa000<=location && location<=0xbfff) {
        enabled = true;
    } else if (0xe000<=location && location <=0xffff) {
        memcpy(&cpu->memory[0xc000],cpu->rom->get_prg_bank((uint8_t)(*value)<<3),0x2000);
    }
}

void NTDEC2722::clock(void** ptrs) {
    CPU* cpu = (CPU*)ptrs[0];
    if (enabled) { 
        counter--;
        if (counter==0) {
            cpu->recv_irq = true;
            counter = 4096*3;
        }
    }
}


void MMC1::control(CPU* cpu, PPU* ppu, uint8_t val) {
    uint8_t mirroring = val&0x3;
    NT_MIRROR newmode;
    switch (mirroring) {
        case 0:
        case 1:
            newmode = SINGLESCREEN;
            //printf("singlescreen mirroring\n");
            break;
        case 2:
            newmode = VERTICAL;
            if (ppu->mirrormode == HORIZONTAL) {
                memcpy(&ppu->memory[0x2400],&ppu->memory[0x2800],0x400);
            }
            break;
        case 3:
            newmode = HORIZONTAL;
            if (ppu->mirrormode == VERTICAL) {
                memcpy(&ppu->memory[0x2800],&ppu->memory[0x2400],0x400);
            }
            //printf("horizontal mirroring\n");
            break;
    }
    if (newmode!=ppu->mirrormode) {
        ppu->mirrormode = newmode;
    }
    prg_mode = ((val&0xc)>>2)&0x3;
    chr_mode = ((val&0x10)>>4)&0x1;
    if (prg_mode == 2) {
        memcpy(&cpu->memory[0x8000],cpu->rom->get_prg_bank(0),sizeof(uint8_t)*0x4000);
    } else if (prg_mode == 3) {
        memcpy(&cpu->memory[0xC000],cpu->rom->get_prg_bank((cpu->rom->get_prgsize()/0x400)-16),sizeof(uint8_t)*0x4000);
    }

}

void MMC1::map_write(void** ptrs, int8_t* address, int8_t* value) {
    CPU* cpu = (CPU*)ptrs[0];
    ROM* rom = cpu->rom;
    PPU* ppu = (PPU*)ptrs[1];
    int8_t val = *value;
    long long location = address-(cpu->memory);
    if (location >= 0x8000 && location <= 0xFFFF) {
        if (val&0x80) {
            shift_reg = 0x10;
            control(cpu,ppu,0xc);
        } else {
            uint8_t cp = shift_reg&1;
            shift_reg >>=1;
            shift_reg |= (val&1)<<4;
            if (cp) {
                bank_reg = shift_reg;
                if (location>=0x8000 && location <= 0x9FFF) {
                    control(cpu,ppu,bank_reg);
                    //printf("MMC1 Control: ");
                } else if (location >= 0xA000 && location <= 0xBFFF) {
                    //printf("chr-ram: %p, ppu memory: %p\n",rom->chr_ram,ppu->memory);
                    memcpy(ppu->memory,rom->get_chr_bank((uint8_t)((bank_reg&(~(!chr_mode))))<<2),sizeof(uint8_t)*(0x1000)<<(!chr_mode));
                    //printf("MMC1 CHR BANK 1: ");
                } else if (location >= 0xC000 && location <= 0xDFFF && chr_mode) {
                    //printf("chr-ram: %p, ppu memory: %p\n",rom->chr_ram,ppu->memory);
                    memcpy(&ppu->memory[0x1000],rom->get_chr_bank((uint8_t)(bank_reg<<2)),sizeof(uint8_t)*0x1000);
                    //printf("MMC1 CHR BANK 2: ");
                } else if (location >= 0xE000 && location <= 0xFFFF) {
                    //printf("MMC1 PRG BANK: ");
                    switch (prg_mode) {
                        case 0:
                        case 1:
                            memcpy(&cpu->memory[0x8000],rom->get_prg_bank((uint8_t)(bank_reg&(~1))<<4),sizeof(uint8_t)*0x8000);
                            break;
                        case 2:
                            memcpy(&cpu->memory[0x8000],cpu->rom->get_prg_bank(0),sizeof(uint8_t)*0x4000);
                            memcpy(&cpu->memory[0xC000],rom->get_prg_bank((uint8_t)bank_reg<<4),sizeof(uint8_t)*0x4000);
                            break;
                        case 3:
                            memcpy(&cpu->memory[0x8000],rom->get_prg_bank((uint8_t)bank_reg<<4),sizeof(uint8_t)*0x4000);
                            memcpy(&cpu->memory[0xC000],cpu->rom->get_prg_bank(cpu->rom->get_prgsize()/0x400-16),sizeof(uint8_t)*0x4000);
                            break;
                    }
                }
                //printf("%02x\n",bank_reg);
                shift_reg = 0x10;
            }
        }
    }
}