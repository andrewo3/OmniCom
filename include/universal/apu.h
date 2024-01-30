#ifndef APU_H
#define APU_H
#include <cstdint>
#include "cpu.h"
#include "util.h"

class CPU;

class APU {
    public:
        ~APU();
        void setCPU(CPU* c_ptr);
        void cycle(); 
        void send();// final output (modify current_out variable and send to audio callback)
        long long start = epoch();
        long long cycles = 0;
        SDL_AudioDeviceID device;
        int sample_adj = 0;
        int16_t* audio_buffer = new int16_t[BUFFER_LEN];

        CPU* cpu;
        long long audio_frame = 0;
        long long audio_sent = 0;
        uint8_t step = 0;
        int buffer_size = 0;

        //pulse channels
        void pulse(bool ind);
        uint8_t pulse_out[2] = {0,0};
        bool pulse_waveforms[4][8] = {
            {0,1,0,0,0,0,0,0},
            {0,1,1,0,0,0,0,0},
            {0,1,1,1,1,0,0,0},
            {1,0,0,1,1,1,1,1}
        }; //choose different one based on duty
        uint16_t pulse_timer[2] = {0,0};

        //envelopes
        uint8_t env[3][3] = {
            {0,0,0},
            {0,0,0},
            {0,0,0}
        }; // arranged: start flag, divider, decay level - one for pulse 1 & 2, and noise


        //triangle channel info
        uint8_t tri[6] = {
            0,0,0,0,0,0
        }; //arranged: timer, length counter, linear counter, linear reload flag, control flag, sequencer 

        //length counters
        uint8_t length_counter[4] = {
            0,0,0,0
        }; // one length counter for each channel - arranged: pulse 1, pulse 2, triangle, noise

        //sweep unit info
        uint8_t sweep_units[2][3] = {
            {0,0,0},
            {0,0,0}
        }; //one for each pulse channel, arranged: divider, reload flag, mute channel

        int8_t* FRAME_COUNTER;
        bool frame_interrupt = false;
        int16_t current_out = 0;
        int sample_rate = 0;
        long long frames = 0;
        long long last_aud_frame = 0;
        void setSampleRate(int sr) {sample_rate = sr;}
        uint8_t length_lookup(uint8_t in);
    private:
        void clock_envs();
        void clock_linear();
        void clock_length();
        void clock_sweep();
        void func_frame_counter();
        uint16_t get_pulse_period(bool ind);
        void set_pulse_period(uint16_t val, bool ind);

};

int16_t mix(APU* a_ptr);



#endif