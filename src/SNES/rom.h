#ifndef SNES_ROM_H
#define SNES_ROM_H
#include <cstdint>

namespace SNES {
class ROM {
    public:
        ROM();
        enum types {LOROM,HIROM,EXHIROM};
        bool low_hi;
        char name[21];
        uint8_t type;
        uint8_t chipset;
        long long rom_size_kb;
        long long ram_size_kb;
        uint8_t country;
        void map(uint32_t &address);
        void place_header(uint8_t* data);
};
}

#endif