#include "util.h"
#include "apu.h"
#include <cstdint>
#include <cmath>


void APU::setCPU(CPU* c_ptr) {
    cpu = c_ptr;
    FRAME_COUNTER = &cpu->memory[0x4017];
}

int8_t APU::pulse(bool index) {
    long long pulse_frame = audio_frame;
    pulse_frame %=sample_rate;
    uint16_t t = (cpu->memory[0x4002+4*index]&0xff)|(((cpu->memory[0x4003+4*index])&0x7)<<8);
    int frequency = cpu->CLOCK_SPEED/(16*(t+1));
    float duty = pulse_duty[index] ? .25*pulse_duty[index] : .125;
    if (frequency!=0 && t>=8 && pulse_lengths[index]>0) {
        float period = sample_rate/(float)frequency;
        float diff = pulse_frame/period - floorf(pulse_frame/period);
        return pulse_vols[index]*((diff>=duty)*2-1);
    } else {
        return 0;
    }

}

int8_t APU::tri() {
    //if index == 0, its pulse 1
    //if index == 1, its pulse 2
    uint16_t t = cpu->memory[0x400A]|((cpu->memory[0x400B]&0x7)<<8);
    int frequency = cpu->CLOCK_SPEED/(16*(t+1));

    int ratefreq = sample_rate/frequency;
    //double x = (double)frame/sample_rate;
    float triangle_value = 0;
    return tri_vol*(triangle_value);

}

int16_t mix(APU* a_ptr) {
    int8_t pulse1 = a_ptr->pulse(false);
    int8_t pulse2 = a_ptr->pulse(true);
    a_ptr->audio_frame++;
    int16_t output = ((int16_t)32767*(pulse1/15.0+pulse2/15.0));
    //printf("out: %f\n", (float)output/32767);
    return output;
}

void APU::cycle() { //frame counter quarter frame
    FRAME_COUNTER = &cpu->memory[0x4017];
    bool step5 = ((*FRAME_COUNTER)&0x80);
    bool inhibit = ((*FRAME_COUNTER)&0x40);

    // update frame_counter variable
    for (int i=0; i<2; i++) {
        if (pulse_lengths[i]>0) {
            if (!pulse_halt[i]) { //if length counter halt for pulse hasnt been set
                pulse_lengths[i]--;
            }
            /*if (!pulse_const[i]) {
                if (pulse_vols[i]>0) {
                    pulse_vols[i]--;
                }
            }*/
        }
    }
    last_aud_frame = audio_frame;
    //printf("STEP %i\n",step);
    //audio_buffer.push(0);
    frames++;
}

uint8_t APU::length_lookup(uint8_t in) {
    // from: https://www.nesdev.org/wiki/APU_Length_Counter#Table_structure
    uint8_t low4 = in&0x10;
    if (in&1) {
        if (in!=1) {
            return in-1;
        } else {
            return 254;
        }
    } else if ((in&0xF)<=0x8) {
        return (10|(low4>>3))<<((in&0xF)>>1);
    } else if ((in&0xF)>0x8) {
        switch (in&0xF) {
            case 0xA:
                return low4 ? 72 : 60;
            case 0xC:
                return low4 ? 16 : 14;
            case 0xE:
                return low4 ? 32 : 26;
        }
    }
}