#include "rom.h"
#include "cpu.h"
#include "ppu.h"
#include "mapper.h"
#include <cstdint>
#include <filesystem>
#include <cstdio>
#include <cstring>

// description of addressing modes:
// https://blogs.oregonstate.edu/ericmorgan/2022/01/21/6502-addressing-modes/
//Addressing modes more in depth: https://wiki.cdot.senecacollege.ca/wiki/6502_Addressing_Modes
//Flags: https://www.nesdev.org/wiki/Status_flags
//Memory Map: https://www.nesdev.org/wiki/CPU_memory_map
//Opcode Table: https://www.masswerk.at/6502/6502_instruction_set.html#layout
//Instruction Descriptions: https://www.nesdev.org/obelisk-6502-guide/reference.html
//program execution steps: https://www.middle-engine.com/blog/posts/2020/06/23/programming-the-nes-the-6502-in-detail
using namespace NES; 

CPU::CPU() {
    define_opcodes();
    define_timings();
}

CPU::CPU(bool dbug) {
    this->debug = dbug;
    define_opcodes();
    define_timings();
}

void CPU::start_nmi() {
    //printf("NMI\n");
    recv_nmi = false;
    uint16_t push = get_addr(pc);
    stack_push((uint8_t)(push>>8));
    stack_push((uint8_t)(push&0xff));
    stack_push(flags);
    set_flag('I',true);

    int8_t * res = &memory[NMI];
    pc = abs(res); 
}

void CPU::start_irq() {
    //printf("NMI\n");
    
    if (!get_flag('I')) {
        recv_irq = false;
        //printf("IRQ Started: CPU %i!\n",cycles);
        uint16_t push = get_addr(pc);
        stack_push((uint8_t)(push>>8));
        stack_push((uint8_t)(push&0xff));
        stack_push(flags);
        set_flag('I',true);

        int8_t * res = &memory[IRQ];
        pc = abs(res); 
    }
}

void CPU::write(int8_t* address, int8_t value) {
    map_memory(&address);
    long long mem = get_addr(address); 
    switch(mem) {
        //write to OAMADDR (0x2001) is handled implicitly
        //write to OAMADDR (0x2003) is handled implicitly
        case 0x2003:
            ppu->oam_addr = (uint8_t)value;
            break;
        case 0x2000:
            //printf("(Before) Write %02x->0x%04x: v=%04x,t=%04x,w=%i,x=%02x\n",value&0xff,mem,ppu->v,ppu->t,ppu->w,ppu->x);
            ppu->t &= ~0xC00;
            ppu->t |= (value&0x3)<<10;
            //printf("(After) Write %02x->0x%04x: v=%04x,t=%04x,w=%i,x=%02x\n",value&0xff,mem,ppu->v,ppu->t,ppu->w,ppu->x);
            break;
        case 0x2004: //write to OAMDATA
            if (ppu->oam_addr%4==2) {
                value &= ~0x1C;
            }
            ppu->oam[ppu->oam_addr] = value&0xff;
            ppu->oam_addr++;
            break;
        case 0x2005: //write to PPUSCROLL
            //printf("(Before) Write %02x->0x%04x: v=%04x,t=%04x,w=%i,x=%02x\n",value&0xff,mem,ppu->v,ppu->t,ppu->w,ppu->x);
            if (!ppu->w) {
                ppu->t &= ~0x1F; //mask for bit replacement
                ppu->t |= (((uint8_t)value)>>3)&0x1F;
                ppu->x = value&0x7;
                ppu->w = 1;
            } else {
                ppu->t &= ~0x73e0; //another bit mask for replace bits;
                ppu->t |= (value&0xf8)<<2;
                ppu->t |= (value&0x7)<<12;
                ppu->w = 0;
            }
            //printf("(After) Write %02x->0x%04x: v=%04x,t=%04x,w=%i,x=%02x\n",value&0xff,mem,ppu->v,ppu->t,ppu->w,ppu->x);
            break;
        case 0x2006: //write to PPUADDR
            if (!ppu->w) {
                ppu->t &= ~0x3F00;
                ppu->t |= (value&0x3f)<<8;
                ppu->t &= ~0x4000;
                ppu->w = 1;
            } else {
                ppu->t &= ~0xff;
                ppu->t |= value&0xff;
                ppu->v = ppu->t;
                ppu->address_bus = ppu->v;
                ppu->w = 0;
            }
            break;
        case 0x2007: // write to PPUDATA
            {
            //printf("(Before) Write %02x->0x%04x: v=%04x,t=%04x,w=%i,x=%02x\n",value&0xff,mem,ppu->v,ppu->t,ppu->w,ppu->x);
            uint16_t bit14 = (ppu->v)&0x3fff;
            if (debug) {
                printf("ppu->%04x: %02x\n",bit14,(uint8_t)value);
            }
            ppu->write(bit14,value); // write method takes mapper into account
            ppu->v+=(memory[0x2000]&0x04) ? 0x20 : 0x01;
            if (debug) {
                printf("v changed to %i\n",ppu->v);
            }
            ppu->address_bus = ppu->v;
            //ppu->address_bus=ppu->v;
            //ppu->v&=~0x3fff;
            //ppu->v|=bit14;
            //printf("(After) Write %02x->0x%04x: v=%04x,t=%04x,w=%i,x=%02x\n",value&0xff,mem,ppu->v,ppu->t,ppu->w,ppu->x);
            break;
            }
        case 0x4000:
            //apu->env[0][0] = 1; // set envelope start flag
            break;
        case 0x4001:
            apu->sweep_units[0][1]=1; //set sweep unit reload flag
            break;
        case 0x4002: //Pulse 1 timer Low
            apu->pulse_periods[0]=(value&0xff)|((memory[0x4003]&0x7)<<8);
            break;
        case 0x4003:
            apu->length_counter[0] = apu->length_lookup((value&0xF8)>>3);
            apu->pulse_periods[0]=(memory[0x4002]&0xff)|((value&0x7)<<8);
            apu->env[0][0] = 1; // set envelope start flag
            break;
        case 0x4004:
            //apu->env[1][0] = 1; // set envelope start flag
            break;
        case 0x4005:
            apu->sweep_units[1][1]=1; //set sweep unit reload flag
            break;
        case 0x4006: //Pulse 2 timer low
             apu->pulse_periods[1]=(value&0xff)|((memory[0x4007]&0x7)<<8);
            break;
        case 0x4007:

            apu->length_counter[1] = apu->length_lookup((value&0xF8)>>3);
            apu->pulse_periods[1]=(memory[0x4006]&0xff)|((value&0x7)<<8);
            apu->env[1][0] = 1; // set envelope start flag
            break;
        case 0x400A:
            apu->tri_period=(value&0xff)|((memory[0x400B]&0x7)<<8);
            break;
        case 0x400B:
            apu->tri_period=(memory[0x400A]&0xff)|((value&0x7)<<8);
            apu->length_counter[2] = apu->length_lookup((value&0xF8)>>3);
            apu->linear_reload = true; // set triangle linear counter reload flag
            break;
        case 0x400F:
            apu->length_counter[3] = apu->length_lookup((value&0xF8)>>3);
            apu->env[2][0] = 1; //set envelope start flag
            break;
        case 0x4010:
            apu->dmc_flags = value;
            break;

        case 0x4011:
            apu->set_dmc = value&0xff;
            break;

        case 0x4012:
            apu->sample_address = 0xC000|((value&0xff)<<6);
            break;
        
        case 0x4013:
            apu->sample_length = ((value&0xff)<<4)|1;
            break;
        
        case 0x4014: //write to OAMDMA
            //memcpy(ppu->oam,&memory[(uint16_t)((value&0xff)<<8)],256);
            {uint8_t old_oam = ppu->oam_addr;
            for (int i=0; i<256; i++) {
                ppu->oam[(uint8_t)(ppu->oam_addr+i)] = read(&memory[(uint16_t)((value&0xff)<<8)+i])&0xff;
                if (i%4==2) {
                    ppu->oam[i] &= ~0x1C;
                }
                ppu->oam[i]&=0xff;
            }
            ppu->oam_addr = old_oam;
            if (debug) {
                printf("New OAM: [");
                for (int i=0; i<256; i++) {
                    printf("%02x,",ppu->oam[i]);
                }
                printf("]\n");
            }
            break;}
        case 0x4015: //APU Status
            for (int i=0; i<=4; i++) {
                if (!(value&(1<<i))) {
                    apu->enabled[i] = 0;
                    if (i<4) {
                        apu->length_counter[i] = 0;
                    } else {
                        apu->sample_bytes_remaining = 0;
                    }
                } else {
                    apu->enabled[i] = 1;
                    if (i==4 && apu->sample_bytes_remaining==0) {
                        apu->start_sample();
                    }
                }
            }
            break;
        case 0x4016: //controller input 1
            input_strobe = value&1;
            if (input_strobe) {
                //poll input
                inputs[0] = conts[0] == nullptr ? 0 : conts[0]->get_input_byte();
                inputs[1] = conts[1] == nullptr ? 0 : conts[1]->get_input_byte();
            }
            break;
        case 0x4017:
            apu->timer_reset = apu->cycles-1; //reset apu timer
            if (value&0x80) {
                apu->clock_envs();
                apu->clock_length();
                apu->clock_linear();
                apu->clock_sweep();
            }
            break;
    }
    // special mapper cases
    void* system[3] = {this,ppu,apu};
    rom->get_mapper()->map_write(&system[0],address,&value);

    if (!(0x8000<=mem && mem<=0xffff)) {
        if (mem!=0x2002) {
            *address = value;
        }
    }
}

void CPU::set_controller(Controller* cont, uint8_t port) {
    conts[port] = cont;
}

int8_t CPU::read(int8_t* address, bool from_cpu) {
    map_memory(&address);
    uint16_t mem = get_addr(address);
    int8_t value = *address;
    switch(mem) { // handle special ppu and apu registers
        case 0x2002:
            {
            *address &= 0x7F;
            ppu->w = 0;
            //data access happens on last cycle, so
            //|| ppu->scycle>339 && ppu->scanline==240
            if (ppu->scycle>=2 && ppu->scanline==261) {
                value &= 0x7f;
            } else if (ppu->scycle==1 && ppu->scanline==241) {
                ppu->disable_vbl = true;
                ppu->nmi_suppress = true;
                ppu->nmi_occurred = false;
                value &= 0x7f;
            }
            else if (ppu->scycle==2 && ppu->scanline==241) {
                ppu->disable_vbl = true;
                ppu->nmi_suppress = true;
                ppu->nmi_occurred = false;
                value|=0x80;
            } else {
                ppu->nmi_occurred = false;
            }
            break;
            }
        case 0x2004: //OAMDATA
            value = ppu->oam[ppu->oam_addr];
            break;
        case 0x2007: //PPUDATA
            {
            uint16_t bit14 = (ppu->v)&0x3fff;
            if (bit14<=0x3eff) {
                value = ppu->read_buffer;
                ppu->read_buffer = ppu->read(bit14);
            } else {
                value = ppu->read(bit14);
            }
            ppu->v+=(memory[0x2000]&0x04) ? 0x20 : 0x01;
            
            //ppu->v %= 0x4000;
            break;
            }
        case 0x4015:
            value = (apu->frame_interrupt&0x40);
            for (int i=0; i<4; i++) {
                value|=(apu->length_counter[i]>0)&(1<<i);
            }
            apu->frame_interrupt = false;
            break;
        case 0x4016:
            if (!input_strobe) {
                value = (inputs[0]&0x80)>>7;
                inputs[0]<<=1;
                inputs[0]|=1;
            } else {
                value = (inputs[0]&0x80)>>7;
            }

            value &= ~0xF8;
            value |= 0x40;
            break;
        case 0x4017:
            if (!input_strobe) {
                value = (inputs[1]&0x80)>>7;
                inputs[1]<<=1;
                inputs[1]|=1;
            } else {
                value = (inputs[1]&0x80)>>7;
            }
            value &= ~0xF8;
            value |= 0x40;
            break;

    }
    if (from_cpu) {
        void* system[3] = {this,ppu,apu};
        rom->get_mapper()->map_read(&system[0],address);
    }
    return value;
}

void CPU::ins_str(char * write,int buf_size,uint8_t opcode) {
    if (debug_opcodes[opcode]!=nullptr && debug_addr[opcode]!=nullptr) {
        snprintf(write,buf_size,"0x%02x: %s, %s, PC=$%04llx - A=%u - X=%u - Y=%u",
        opcode,
        this->debug_opcodes[opcode],
        this->debug_addr[opcode],
        get_addr(pc),
        accumulator,
        x,
        y);
    } else {
        snprintf(write,buf_size,"0x%02x: ---",opcode);
    }
}

void CPU::ins_str_mem(char * write,int buf_size,uint8_t* mem,int8_t* arg_ptr) {
    map_memory((int8_t**)&mem);
    map_memory(&arg_ptr);
    uint8_t opcode = mem[0];
    uint16_t a;
    if (ins_size<=3) {
        memcpy(&a,&mem[1],ins_size-1);
    } else {
        a = mem[1];
    }
    if (debug_opcodes[opcode]!=nullptr && debug_addr[opcode]!=nullptr) {
        snprintf(write,buf_size,"Cycles: %lli, 0x%02x: %s, %s $%04x->%04llx=%02x, PC=$%04llx - A=%02x - X=%02x - Y=%02x - P=%02x",
        cycles,
        opcode,
        this->debug_opcodes[opcode],
        this->debug_addr[opcode],
        a,
        get_addr(arg_ptr),
        (uint8_t)*arg_ptr,
        get_addr(pc),
        (uint8_t)accumulator,
        (uint8_t)x,
        (uint8_t)y,
        flags);
    } else {
        snprintf(write,buf_size,"0x%02x: ---",opcode);
    }
}

void CPU::map_memory(int8_t** address) {
    Mapper* m = rom->get_mapper();
    long long addr = get_addr(*address);
    switch(m->type) {
        case 0:
            if (rom->get_prgsize()/0x4000==1) {
                if (0xC000<=addr && addr<=0xFFFF) {
                    *address-=0x4000; //mirror first table
                }
            }
            break;
        case 3:
            if (rom->get_prgsize()/0x4000==1) {
                if (0xC000<=addr && addr<=0xFFFF) {
                    *address-=0x4000; //mirror first table
                }
            }
            break;
    }
    if (0x0800<=addr && addr < 0x2000) {
        *address-=addr/0x800*0x800;
    } else if (0x2008<=addr && addr < 0x4000) {
        *address-=(addr-0x2000)/0x8*0x8;
    }
}

void CPU::clock() {
    ins_size = 1;
    int8_t* ins = pc;
    for (int loc: breakpoints) {
        if (ppu->scanline == 240 && ppu->frames==9) {
            break;
        }
        if (pc-memory == loc) {
            break; //put actual breakpoint here with debugger to stop at every cpu breakpoint
        }
    }
    //map_memory(&ins);
    uint8_t ins_value = read(ins);
    //cycles+=inst_cycles[ins_value];
    instruction exec = this->opcodes[ins_value]; // get instruction from lookup table
    addressing_mode addr = this->addrmodes[ins_value]; // get addressing mode from another lookup table
    int8_t* arg = &ins[1];
    bool xpage = ((uint16_t)(read(arg)&0xff)+(uint16_t)(x&0xff)) > 0xff;
    bool ypage = ((uint16_t)(read(arg)&0xff)+(uint16_t)(y&0xff)) > 0xff;
    if (addr!=nullptr) {
        arg = (this->*addr)(&ins[1]); // run addressing mode on raw value from rom
    }
    if ((addr==&CPU::absx && xpage)||((addr==&CPU::absy || addr==&CPU::indy) && ypage)) {
        current_cycles = inst_cycles_pagecross[ins_value];
    } else {
        current_cycles = inst_cycles[ins_value];
    }
    cycles += current_cycles;

    //project what ppu values will be by the time ppu is executing
    //(they are supposed to be in parallel with each other)
    nmi_output = memory[0x2000]&0x80;
    bool nmi_now = read_nmi && nmi_output;
    /*for (int i=0; i<3*(current_cycles-1); i++) { //catch ppu up to speed
        ppu->cycle();
    }
    ppu->cycle();*/
    read_nmi = ppu->nmi_occurred;
    /*for (int i=0; i<2; i++) { //catch ppu up to speed
        ppu->cycle();
    }*/
    /*int perceived_cycle = ppu->scycle+3*(current_cycles-1);
    int perceived_scanline = ppu->scanline;
    if (perceived_cycle>340) {
        perceived_scanline++;
        perceived_cycle%=341;
    }
    if (perceived_scanline==261 && perceived_cycle>=0) { //projected end of vblank
        projected_nmi = false;
    } else if (ppu->scanline<241 && perceived_scanline == 241 && perceived_cycle > 2 && !ppu->nmi_suppress) {
        projected_nmi = true;
    } else {
        projected_nmi = ppu->nmi_occurred;
    }*/
    /*if (!(!(ins_value&0xf) && (ins_value&0x10)) && ins_value<0xE4) { // if not a branch instruction
        cycles--;
    }*/
    //map_memory(&arg); // update banks and registers as needed
    if (debug) { //print instruction
        const int buf_size = 256;
        char w[buf_size] = {0};
        ins_str_mem(w,buf_size,(uint8_t*)ins,arg);
        printf("%s ",w);
        //print stack
        printf("SP: %02x [",sp);
        if (sp!=0xff) {
            printf("%02x",(uint8_t)memory[0x01ff]);
        }
        for (int i=0xfe; i>sp; i--) {
            printf(",%02x",(uint8_t)memory[0x100+i]);
        }
        printf("]\n");
    }
    (this->*exec)(arg); // execute instruction
    ins_num++;
    pc+=ins_size; // increment by instruction size (determined by addressing mode)
    if (pc-memory>=0x10000) {
        pc = memory+(pc-memory)%0x10000;
    }
    if (nmi_now && last_nmi == false) {
        start_nmi();
    }
    else if (recv_irq) {
        start_irq();
    }
    last_nmi = nmi_now;

    
}

int CPU::emulated_clock_speed(long long elapsed) {
    if (cycles!=0 && elapsed!=0) {
        return cycles*1e9/elapsed;
    } else {
        return 0;
    }
}

void CPU::reset() {
    cycles = 0;
    ppu->cycles = 0;
    apu->cycles = 0;
    apu->audio_frame = 0;
    int8_t * res = &memory[RESET];
    map_memory(&res);
    pc = abs(res);
    //for test purposes: remove this later.
    //pc = &memory[0xc000];
}

void CPU::loadRom(ROM *r,bool use_ram) {
    for (int i=0; i<0x10000; i++) {
        memory[i] = 0;
    }
    rom = r;
    Mapper* m = rom->get_mapper();
    if (rom->battery_backed && use_ram) {
        printf("load RAM file: %s\n",(config_dir+sep+std::string("ram")).c_str());
        printf("RAM file exists: %i\n",std::filesystem::exists(config_dir+sep+std::string("ram")));
        if (std::filesystem::exists(config_dir+sep+std::string("ram"))) {
            FILE* ram = fopen((config_dir+sep+std::string("ram")).c_str(),"rb");
            load_ram(ram);
            fclose(ram);
        }
    }
    //printf("test %i\n",rom->get_prgsize());
    //printf("CPU PRG SIZE: %i\n",rom->get_prgsize());
    switch(m->type) {
        case 0:
            memcpy(&memory[0x8000],rom->prg,rom->get_prgsize());
            break;
        case 1:
            memcpy(&memory[0x8000],rom->get_prg_bank(0),0x4000);
            memcpy(&memory[0xc000],rom->get_prg_bank((rom->get_prgsize()/0x400)-16),0x4000);
            break;
        case 2:
            memcpy(&memory[0x8000],rom->get_prg_bank(prg_bank_num),0x4000);
            memcpy(&memory[0xc000],rom->get_prg_bank((rom->get_prgsize()/0x400)-16),0x4000);
            break;
        case 3:
            memcpy(&memory[0x8000],rom->prg,rom->get_prgsize());
            break;
        case 4:
            memcpy(&memory[0xC000],rom->get_prg_bank((rom->get_prgsize()/0x400)-16),0x4000);
            break;
        case 40:
            memcpy(&memory[0x6000],rom->get_prg_bank(6<<3),0x2000);
            memcpy(&memory[0x8000],rom->get_prg_bank(4<<3),0x2000);
            memcpy(&memory[0xa000],rom->get_prg_bank(5<<3),0x2000);
            memcpy(&memory[0xe000],rom->get_prg_bank(7<<3),0x2000);
            break;
    }

}

long long CPU::get_addr(int8_t* ptr) {
    return ptr-memory;
}

bool CPU::get_flag(char flag) {
    switch(flag) {
        case 'C':
            return flags&0x1;
        case 'Z':
            return flags&0x2;
        case 'I':
            return flags&0x4;
        case 'D':
            return flags&0x8; //this flag is disabled on NES 6502
        case 'B':
            return flags&0x10;
        case 'V':
            return flags&0x40;
        case 'N':
            return flags&0x80;
        default:
            return 0;
    }
}

void CPU::set_flag(char flag,bool val) {
    if (val) {
        switch(flag) {
            case 'C':
                flags|=0x01;
                break;
            case 'Z':
                flags|=0x02;
                break;
            case 'I':
                flags|=0x04;
                break;
            case 'D':
                flags|=0x08;
                break;
            case 'B':
                flags|=0x10;
                break;
            case 'V':
                flags|=0x40;
                break;
            case 'N':
                flags|=0x80;
                break;
        }
    } else {
        switch(flag) {
            case 'C':
                flags&=0xFE;
                break;
            case 'Z':
                flags&=0xFD;
                break;
            case 'I':
                flags&=0xFB;
                break;
            case 'D':
                flags&=0xF7;
                break;
            case 'B':
                flags&=0xEF;
                break;
            case 'V':
                flags&=0xBF;
                break;
            case 'N':
                flags&=0x7F;
                break;
        }
    }
}

void CPU::save_state(FILE* save_file) {
    fwrite(&flags,sizeof(flags),1,save_file);
    fwrite(&accumulator,sizeof(accumulator),1,save_file);
    fwrite(&x,sizeof(x),1,save_file);
    fwrite(&y,sizeof(y),1,save_file);
    fwrite(&sp,sizeof(sp),1,save_file);
    long long pc_loc = get_addr(pc);
    fwrite(&pc_loc,sizeof(long long),1,save_file);
    fwrite(memory,sizeof(int8_t),0x10000,save_file);
    fwrite(ppu->memory,sizeof(int8_t),0x4000,save_file);
    char mapper[256];
    void* system[3] = {this,ppu,apu};
    rom->get_mapper()->serialize(&system[0],&mapper[0]);
    fwrite(mapper,sizeof(char),256,save_file);
}

void CPU::load_state(FILE* save_file) {
    fread(&flags,sizeof(flags),1,save_file);
    fread(&accumulator,sizeof(accumulator),1,save_file);
    fread(&x,sizeof(x),1,save_file);
    fread(&y,sizeof(y),1,save_file);
    fread(&sp,sizeof(sp),1,save_file);
    long long pc_loc;
    fread(&pc_loc,sizeof(long long),1,save_file);
    pc = memory+pc_loc;
    fread(memory,sizeof(int8_t),0x10000,save_file);
    fread(ppu->memory,sizeof(int8_t),0x4000,save_file);
    char mapper[256];
    fread(mapper,sizeof(char),256,save_file);
    void* system[3] = {this,ppu,apu};
    rom->get_mapper()->deserialize(&system[0],&mapper[0]);
}

void CPU::save_ram(FILE* save_file) {
    fwrite(&memory[0x6000],sizeof(uint8_t),0x2000,save_file);
}

void CPU::load_ram(FILE* save_file) {
    fread(&memory[0x6000],sizeof(uint8_t),0x2000,save_file);
}

void CPU::stack_push(int8_t val) {
    memory[0x0100+sp] = val;
    sp--;
}

uint8_t CPU::stack_pull(void) {
    sp++;
    //printf("Top of stack (%02x): %02x\n",sp,(uint8_t)memory[0x0100+sp]);
    return memory[0x0100+sp]; 
}

