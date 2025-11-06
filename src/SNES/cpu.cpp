#include "cpu.h"
#include <cstdio>
#include <vector>
#if defined(__APPLE__) || defined(__unix__)
    #include <sys/mman.h>
#elif defined(__WIN32__)
    
#endif

using namespace SNES;

CPU::CPU() {
    
}

uint8_t CPU::read(uint32_t addr) {
    uint32_t rom_addr = rom->map(addr);
    return rom->mem[rom_addr];
}

bool CPU::get_flag(char flag) {
    switch (flag) {
        case 'N':
            return (bool)(flag&0x80);
        case 'V':
            return (bool)(flag&0x40);
        case 'M':
            return (bool)(flag&0x20);
        case 'X':
            return (bool)(flag&0x10);
        case 'B':
            return (bool)(flag&0x10);
        case 'D':
            return (bool)(flag&0x8);
        case 'I':
            return (bool)(flag&0x4);
        case 'Z':
            return (bool)(flag&0x2);
        case 'C':
            return (bool)(flag&0x1);
        case 'E':
            return this->e;
    }
    return false;
}

void CPU::set_flag(char flag, bool set) {
    if (set) {
        switch (flag) {
            case 'N':
                this->P |= 0x80;
                return;
            case 'V':
                this->P |= 0x40;
                return;
            case 'M':
                this->P |= 0x20;
                return;
            case 'X':
                this->P |= 0x10;
                return;
            case 'B':
                this->P |= 0x10;
                return;
            case 'D':
                this->P |= 0x8;
                return;
            case 'I':
                this->P |= 0x4;
                return;
            case 'Z':
                this->P |= 0x2;
                return;
            case 'C':
                this->P |= 0x1;
                return;
            case 'E':
                this->e = true;
                return;
        }
    } else {
        switch (flag) {
            case 'N':
                this->P &= 0x7f;
                return;
            case 'V':
                this->P &= 0xbf;
                return;
            case 'M':
                this->P &= 0xdf;
                return;
            case 'X':
                this->P &= 0xef;
                return;
            case 'B':
                this->P &= 0xef;
                return;
            case 'D':
                this->P &= 0xf7;
                return;
            case 'I':
                this->P &= 0xfb;
                return;
            case 'Z':
                this->P &= 0xfd;
                return;
            case 'C':
                this->P &= 0xfe;
                return;
            case 'E':
                this->e = false;
                return;
        }
    }
}

void CPU::clock() {
    printf("CPU CLOCK\n");
    auto located = func_cache.find(PC);
    printf("Flags before: %02x %01x\n",this->P, this->e);
    if (located==func_cache.end()) { //if function is not in cache,
        //build new function
        std::vector<uint8_t> f_build;
        build_function(f_build);
        size_t function_size = sizeof(uint8_t)*f_build.size();
        uint8_t* func_out;
        //make buffer to write function into
        #if defined(__APPLE__) || defined(__unix__)
            func_out = (uint8_t*)mmap(NULL,function_size,PROT_READ|PROT_WRITE,MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
        #elif defined(__WIN32__)
            //TODO
        #endif
        //copy function into memory
        memcpy(func_out, f_build.data(),function_size);
        //make buffer executable
        #if defined(__APPLE__) || defined(__unix__)
            mprotect(func_out, function_size, PROT_EXEC);
        #elif defined(__WIN32__)
            //TODO
        #endif
        located->second = (func_branch)func_out;
        located->second(this->P, this->e);

    } else { // execute function from cache
        located->second(this->P, this->e); //pass flags into function
    }
    printf("Flags after: %02x %01x\n",this->P,this->e);
    printf("val at PC: %06x->%02x\n",PC&0xffffff,read(rom->map(PC)));
    PC++;
}

void CPU::reset() {
    printf("CPU RESET\n");
    uint16_t* reset_vec = (uint16_t*)((rom->mem)+rom->map(0xfffc));
    this->PC = *reset_vec;
    this->P = 0x1;
    this->S = 0x1ff;
    this->K = 0x0;
    printf("RESET: %04x->%04x\n",*reset_vec,rom->map(*reset_vec));
}