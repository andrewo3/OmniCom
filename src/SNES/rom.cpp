#include "rom.h"
#include <cstdint>
#include <iostream>

using namespace SNES;

ROM::ROM() {

}


//map address from CPU memory space to ROM memory space based on mapping mode
uint32_t ROM::map(uint32_t address) {
    switch (type) {
        case LOROM:
            {uint32_t bank = address&0x7f0000;
            address&=0x7fff;
            address|=bank>>1;
            return address;}
        case HIROM: //TODO
            printf("unimplemented HiROM\n");
            exit(1);
            break;
        case EXHIROM: //TODO
            printf("unimplemented ExHiROM\n");
            exit(1);
            break;
    }
    return 0; //should never get here
}