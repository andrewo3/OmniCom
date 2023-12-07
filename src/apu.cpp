#include "util.h"
#include "apu.h"
#include <cstdint>


int8_t APU::pulse(bool index) {
    //if index == 0, its pulse 1
    //if index == 1, its pulse 2
    uint8_t D = cpu->memory[0x4000+4*index];
    uint8_t duty = (D&0xC0)>>6;
    bool length_halt = D&0x20;
    bool constant = D&0x10;
    uint8_t volume = D&0xf; // constant vol, and envelope decay speed
    uint16_t t = cpu->memory[0x4002+4*index]|((cpu->memory[0x4003+4*index]&0x7)<<8);
    int frequency = cpu->CLOCK_SPEED/(16*(t+1));
   

    if (!index) {
        return pulse1;
    } else {
        return pulse2;
    }

}

int16_t APU::mixer() {
    bool step5 = (fcmode&0x80);
    long long new_frame_counter = (epoch()-start)/1000.0*240; //240 Hz frame counter
    if (new_frame_counter!=frame_counter) {
        frame_counter = new_frame_counter;
        step++;
        step%=4+step5;
    }
}