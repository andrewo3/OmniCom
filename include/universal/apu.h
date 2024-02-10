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
        long long start = epoch_nano();
        long long cycles = 0;
        long long timer_reset = 0;
        SDL_AudioDeviceID device;
        int sample_adj = 0;
        int16_t* audio_buffer = new int16_t[BUFFER_LEN];

        CPU* cpu;
        long long audio_frame = 0;
        long long audio_sent = 0;
        uint8_t step = 0;
        int buffer_size = 0;

        bool enabled[5] = {0}; //if channels are enabled
        //pulse channels
        void pulse(bool ind);
        int8_t pulse_out[2] = {0,0};
        bool pulse_waveforms[4][8] = {
            {0,1,0,0,0,0,0,0},
            {0,1,1,0,0,0,0,0},
            {0,1,1,1,1,0,0,0},
            {1,0,0,1,1,1,1,1}
        }; //choose different one based on duty
        uint8_t pulse_ind[2] = {0,0};
        uint16_t pulse_timer[2] = {0,0};
        uint16_t pulse_periods[2] = {0,0};

        //triangle channel
        void triangle();
        uint16_t tri_period = 0;
        int8_t tri_sequence[32] = {
            15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,
            0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
        float tri_out = 0;
        uint8_t tri_ind = 0;
        uint16_t tri_timer = 0;

        //envelopes
        uint8_t env[3][3] = {
            {0,0,0},
            {0,0,0},
            {0,0,0}
        }; // arranged: start flag, divider, decay level - one for pulse 1 & 2, and noise


        //triangle channel info
        uint8_t linear_counter = 0;
        bool linear_reload = false;

        //noise
        void noise();
        uint16_t noise_shift = 1;
        int8_t noise_out = 0;
        int noise_periods[16] = {
            4,8,16,32,64,96,128,160,202,254,380,508,762,1016,2034,4068
        };
        int noise_timer = 0;

        //dmc
        void dmc();
        int dmc_out = 0;
        uint8_t sample_buffer = 0;
        uint8_t dmc_flags = 0;
        uint16_t dmc_period_table[0x10] = {
            0x1AC,0x17C,0x154,0x140,0x11E,0x0FE,0x0E2,0x0D6,
            0x0BE,0x0A0,0x08E,0x080,0x06A,0x054,0x048,0x036
        };
        int dmc_timer = 0;
        bool dmc_silence = true;
        uint8_t dmc_shift = 0;
        int dmc_bits_remaining = 0;
        bool sample_empty = true;
        uint16_t sample_address = 0;
        uint16_t sample_length = 0;
        int set_dmc = -1;
        uint16_t current_address = 0;
        int sample_bytes_remaining = 0;
        void start_sample();

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
        void clock_envs();
        void clock_linear();
        void clock_length();
        void clock_sweep();
    private:
        void func_frame_counter();
        uint16_t get_pulse_period(bool ind);
        void set_pulse_period(uint16_t val, bool ind);

};

int16_t mix(APU* a_ptr);



#endif