#include <cstdint>

class PPU {
    public:
        uint8_t* PPUCTRL; //&memory[0x2000]
        uint8_t* PPUMASK; //&memory[0x2001]
        uint8_t* PPUSTATUS; //&memory[0x2002]
        uint8_t* OAMADDR; //&memory[0x2003]
    private:
};