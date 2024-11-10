#include "rom.h"
#include <cstdint>
#include <iostream>

using namespace SNES;

ROM::ROM() {

}

void ROM::map(uint32_t &address) {
    switch (type) {
        case LOROM:
            address |= 0x808000;
            break;
        case HIROM:
            printf("unimplemented HiROM\n");
            address |= 0xc00000;
            exit(1);
            break;
        case EXHIROM:
            printf("unimplemented ExHiROM\n");
            exit(1);
            break;
    }
}

void ROM::place_header(uint8_t* data) {
    //TODO
}