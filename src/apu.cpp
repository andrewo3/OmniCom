#include "util.h"
#include "apu.h"
#include <cstdint>
#include <cmath>


void APU::setCPU(CPU* c_ptr) {
    cpu = c_ptr;
    FRAME_COUNTER = &cpu->memory[0x4017];
}

int8_t APU::pulse(bool index) {
    //if index == 0, its pulse 1
    //if index == 1, its pulse 2
    uint16_t t = cpu->memory[0x4002+4*index]|((cpu->memory[0x4003+4*index]&0x7)<<8);
    int frequency = cpu->CLOCK_SPEED/(16*(t+1));

    int ratefreq = sample_rate/frequency;
    //double x = (double)frame/sample_rate;
    if (ratefreq!=0) {
        float effective_duty = (pulse_duty[index]) ? 0.25*pulse_duty[index]:0.125;
        printf("\n\n--------\n(Pulse: %i\n",index+1);
        printf("Frequency: %i\n",frequency);
        printf("Hold: %i\n",pulse_const[index]);
        printf("Volume (Start Volume if not hold): %i\n",pulse_vols[index]);
        printf("Length Counter: %i\n",pulse_lengths[index]);
        printf("Duty: %.02f%)\n",effective_duty);
        return pulse_lengths[index]>0 ? pulse_vols[index]*((audio_frame%ratefreq)<(effective_duty*ratefreq)) : 0;
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
    if (ratefreq!=0) {
        printf("\n\n!!!!!!!\n(Tri -\n");
        printf("Frequency: %i\n",frequency);
        printf("Hold: %i\n",tri_const);
        printf("Volume (Start Volume if not hold): %i\n",tri_vol);
        printf("Length Counter: %i)\n",tri_length);
        float triangle_value = 0;
        return tri_length>0 ? tri_vol*(triangle_value) : 0;
    } else {
        return 0;
    }

}

int16_t mix(APU* a_ptr) {
    uint8_t pulse1 = a_ptr->pulse(false);
    uint8_t pulse2 = a_ptr->pulse(true);
    uint8_t tri = a_ptr->tri();
    float pulse_out;
    float tnd_out;
    a_ptr->audio_frame++;
    if (!((pulse1+pulse2)==0)) {
            pulse_out = 95.88/((8128.0/(pulse1+pulse2))+100.0);
        } else {
            pulse_out = 0;
        }
        //printf("pulse_out: %f - %i, %i\n",pulse_out,pulse1, pulse2);
    if (!(tri==0)) {
        tnd_out = 159.79/(1/(tri/8227)+100.0);
    } else {
        tnd_out = 0;
    }
    int16_t out = 65535*((pulse_out+tnd_out));
    //printf("%i\n",out);
    //printf("%i\n",audio_buffer.size());
    return out;
        
        
}

void APU::cycle() { //frame counter quarter frame
    bool step5 = ((*FRAME_COUNTER)&0x80);
    bool inhibit = ((*FRAME_COUNTER)&0x40);
    

    // update frame_counter variable
    for (int i=0; i<2; i++) {
        if ((audio_frame-pulse_start[i])/(sample_rate/4)!=(last_aud_frame-pulse_start[i])/(sample_rate/4)) {
            pulse_step[i]++;
            pulse_step[i]%=(4+(int)step5);
            if (pulse_step[i]%2==0) {
                
                if (pulse_lengths[i]>0) {
                    if (!pulse_halt[i]) { //if length counter halt for pulse hasnt been set
                        pulse_lengths[i]--;
                    }
                    if (!pulse_const[i]) {
                        if (pulse_vols[i]>0) {
                            pulse_vols[i]--;
                        }
                    }
                }
                }
            }
        }
    last_aud_frame = audio_frame;
    //printf("STEP %i\n",step);
    //audio_buffer.push(0);
    frames++;
}