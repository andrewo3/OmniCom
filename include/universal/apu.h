#ifndef APU_H
#define APU_H
#include <cstdint>
#include "cpu.h"
#include "util.h"

class CPU;

class APU {
    public:
        void setCPU(CPU* c_ptr);
        void cycle(); 
        void send();// final output (modify current_out variable and send to audio callback)
        long long start = epoch();
        int8_t pulse(bool index);
        int8_t tri();

        CPU* cpu;
        long long audio_frame = 0;
        uint8_t step = 0;

        //pulse channels
        bool pulse_halt[2] = {0,0};
        bool pulse_const[2] = {0,0};
        uint8_t pulse_duty[2] = {0,0};
        uint8_t pulse_start[2] = {0,0};
        uint8_t pulse_step[2] = {0,0};
        uint8_t pulse_vols[2] = {0,0};
        uint8_t pulse_decay_level[2] = {0,0};
        uint8_t pulse_lengths[2] = {0,0};

        //tri channel
        bool tri_halt = 0;
        bool tri_const = 0;
        uint8_t tri_linear = 0;
        uint8_t tri_start = 0;
        uint8_t tri_step = 0;
        uint8_t tri_vol = 0;
        uint8_t tri_decay_level = 0;
        uint8_t tri_length = 0;

        int8_t* FRAME_COUNTER;
        bool frame_interrupt = false;
        uint16_t current_out = 0;
        int sample_rate = 0;
        long long frames = 0;
        long long last_aud_frame = 0;
        void setSampleRate(int sr) {sample_rate = sr;}


};

int16_t mix(APU* a_ptr);

#endif