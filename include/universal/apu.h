#ifndef APU_H
#define APU_H
#include <cstdint>
#include "cpu.h"
#include "util.h"

class CPU;

class APU {
    public:
        int16_t mixer(); // final output (send this to audio callback)
        long long start = epoch();
        int8_t pulse(bool index);
        CPU* cpu;
        long long frame_counter = 0;
        uint8_t p1_count = 0;
        uint8_t p2_count = 0;
        uint8_t pulse1;
        uint8_t pulse2;
        uint8_t step = 0;
        uint8_t fcmode = 0;
    private:


};



#endif