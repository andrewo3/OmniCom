#include "util.h"
#include "apu.h"
#include <cstdint>
#include <cmath>


APU::~APU() {
    delete[] buffer;
    delete[] buffer_copy;
}

APU::APU() {
    for (int i=0; i<BUFFER_LEN; i++) {
        buffer[i] = 0;
    }
}

void APU::setCPU(CPU* c_ptr) {
    cpu = c_ptr;
    clock_speed = c_ptr->CLOCK_SPEED/2;
}

int16_t mix(APU* a_ptr) {
    //pulse1/60.0+pulse2/60.0+
    //a_ptr->audio_frame++;
    int sr = a_ptr->sample_rate;
    bool* en = a_ptr->enabled;
    int8_t p_out = (en[0] ? a_ptr->pulse_out[0] : 0)+(en[1] ? a_ptr->pulse_out[1] : 0);
    float tnd_out = 0.00851*(en[2] ? a_ptr->tri_out : 0) + 0.00494*(en[3] ? a_ptr->noise_out : 0) + 0.00335*(en[4] ? (a_ptr->dmc_out-64)*2 : 0);
    //p_out = 15*((a_ptr->cycles*2*440/(clock_speed))%2);
    float final_vol = 0.00752*p_out+tnd_out;
    int16_t output = final_vol*32767*global_db;
    //output = a_ptr->audio_buffer[(a_ptr->buffer_ind+ind)%BUFFER_LEN];
    //printf("out: %f\n", (float)output/32767);
    return output;
}

void APU::clock_envs() { //clock_envs
    for (int i=0; i<3; i++) {
        int env_addr = 0x4000|(((1<<i)-1)<<2);
        uint8_t* start_flag = &env[i][0];
        uint8_t* divider = &env[i][1];
        uint8_t* decay = &env[i][2]; // ends up determining final volume (or use constant val if flag is set)
        if (!(*start_flag)) {
            if (*divider) {
                (*divider)--;
            } else {
                *divider = cpu->memory[env_addr]&0xf; //reset to V value from corresponding envelope register
                //clock decay counter
                if (*decay) {
                    (*decay)--;
                } else if (cpu->memory[env_addr]&0x20) { // if loop flag set for envelope
                    *decay = 15;
                }
            }
        } else {
            *start_flag = 0;
            *decay = 15;
            *divider = cpu->memory[env_addr]&0xf;
        }
    }
}

void APU::clock_linear() {
    if (linear_reload) {
        linear_counter = cpu->memory[0x4008]&0x7F; // counter reload value
    } else if (linear_counter) {
        linear_counter--;
    }
    if (!(cpu->memory[0x4008]&0x80)) {
        linear_reload = false;
    }
}

void APU::clock_length() {
    for (int l=0; l<4; l++) {
        if (enabled[l]) { // check if channel is enabled using $4015
            bool halt_flag = cpu->memory[0x4000+4*l]&(0x20<<(2*(l==2))); // get corresponding length counter halt flag
            if (!(length_counter[l]==0 || halt_flag)) { // if the length counter is not 0 and the halt flag is not set, decrement
                length_counter[l]--;
            } else if (length_counter[l]==0) {
                //silence corresponding channel
            }
        } else {
            length_counter[l]=0;
        }
    }
}

uint16_t APU::get_pulse_period(bool ind) {
    return (cpu->memory[0x4002|(ind<<2)]&0xff)|((cpu->memory[0x4003|(ind<<2)]&0x7)<<8);
}

void APU::set_pulse_period(uint16_t val, bool ind) { // sets the pulse channel's period to the new value, stored in the appropriate cpu registers (only presumably used by the sweep units)
    cpu->memory[0x4002|(ind<<2)] = val&0xff;
    cpu->memory[0x4003|(ind<<2)] &= ~0x7;
    cpu->memory[0x4003|(ind<<2)] |= (val&0x700)>>8;
}

void APU::clock_sweep() { //clock sweep units
    for (int i=0; i<2; i++) {
        uint8_t* divider = &sweep_units[i][0];
        uint8_t* reload = &sweep_units[i][1];
        uint8_t* muted = &sweep_units[i][2];

        uint8_t sweep_setup = (uint8_t)cpu->memory[0x4001|(i<<2)];
        bool enabled = sweep_setup&0x80;
        uint16_t pulse_period = pulse_periods[i];
        
        //calculate target period
        int16_t change_amount = pulse_period>>(sweep_setup&0x7); // shift current period by shift count in register
        if (sweep_setup&0x8) { //if negative flag
            change_amount = -(change_amount)-i; // the -i is because pulse 2 uses twos complement whereas pulse 1 uses ones, a very minor difference but id like to include it
        }
        int16_t target_period = pulse_period+change_amount;
        if (target_period<0) {
            target_period = 0;
        }

        *muted = (pulse_period<8) || (target_period>0x7ff); // set muted
        
        if ((!(*divider)) && enabled) { // if divider reached zero, sweep is enabled and its not muted
            pulse_periods[i] = target_period; // set pulse period to target period    
        } 
        if ((!(*divider)) || (*reload)) { // check if divider is zero, or reload flag is set
            *divider = (cpu->memory[0x4001|(i<<2)]&0x70)>>4; //reload divider
            *reload = 0; // clear reload
        } else {
            (*divider)--;
        }
    }
}

void APU::func_frame_counter() { //APU frame counter which clocks other update things
    FRAME_COUNTER = &cpu->memory[0x4017];
    bool step5 = ((*FRAME_COUNTER)&0x80);
    bool inhibit = ((*FRAME_COUNTER)&0x40);
    int sequence_clocks = (cycles-timer_reset)%(14916+3724*step5); //14916 is (approximately) the number of apu clocks will occur in 1/60 of a second.
    if (sequence_clocks == 3729) { //step 1 (first quarter frame)
        clock_envs(); // clock envelopes
        clock_linear(); //clock triangle linear counter

    } else if (sequence_clocks == 7458) {//step 2 (second quarter frame)
        clock_envs(); // clock envelopes
        clock_linear(); //clock triangle linear counter

        clock_length(); //clock length counters
        clock_sweep(); //clock sweep units
        

    } else if (sequence_clocks == 11187) { //step 3 (third quarter frame)
        clock_envs(); // clock envelopes
        clock_linear(); //clock triangle linear counter

    } else if (sequence_clocks == 0) {//step 4 (fourth quarter frame)
        clock_envs(); // clock envelopes
        clock_linear(); //clock triangle linear counter

        clock_length(); //clock length counters
        clock_sweep(); //clock sweep units
        if (!step5 && !inhibit) {
            frame_interrupt = true;
        }
    }
}

void APU::cycle() { // apu clock (every other cpu cycle)
    func_frame_counter();
    //everything else
    pulse(0);
    pulse(1);
    //clock triangle twice, because it's clocked per cpu cycle, not apu
    triangle();
    triangle();

    noise();
    dmc();

    if (audio_frame<cycles*sample_rate/clock_speed) {
        if (audio_frame%BUFFER_LEN==0) {
            memcpy(buffer_copy,buffer,sizeof(int16_t)*BUFFER_LEN);
            queue_audio_flag = true;
        }
        buffer[audio_frame%BUFFER_LEN] = mix(this);
        audio_frame++;
    }

    cycles++;
}

void APU::pulse(bool ind) {
    uint16_t period = pulse_periods[ind];
    bool sweep_enabled = (uint8_t)cpu->memory[0x4001|(ind<<2)]&0x80;
    if ((sweep_units[ind][2] && sweep_enabled) || period<8 || length_counter[ind]==0) { //muted channel
        pulse_out[ind] = 0;
    } else {
        uint8_t pulse_reg = cpu->memory[0x4000|(ind<<2)];
        uint8_t duty = (pulse_reg&0xC0)>>6;
        uint8_t volume = pulse_reg&0x10 ? pulse_reg&0xf : env[ind][2];
        pulse_out[ind] = volume*(2*pulse_waveforms[duty][pulse_ind[ind]]-1);

    }
    pulse_timer[ind]++;
    pulse_timer[ind]%=period+1;
    if (!pulse_timer[ind]) {
        pulse_ind[ind]++;
        pulse_ind[ind]%=8;
    }
    //pulse_out[ind] = 15*state[SDL_SCANCODE_R]*(((cycles)*4*440/cpu->CLOCK_SPEED)%2);
}

void APU::triangle() {
    if (linear_counter==0 || length_counter[2]==0 || tri_period<2) {
        tri_out = 0;
    } else {
        tri_out = (tri_sequence[tri_ind]-7.5)*2;
    }
    tri_timer++;
    tri_timer%=tri_period+1;
    if (!tri_timer) {
        tri_ind++;
        tri_ind%=32;
    }
}

void APU::noise() {
    // clock shift register - generating pseudo-random sequence
    if (noise_timer==0) {
        bool mode = cpu->memory[0x400E]&0x80;
        int other_bit = mode ? 6 : 1;
        noise_shift&=~0x8000;
        noise_shift|=((noise_shift&1)^((noise_shift&(1<<other_bit))>>other_bit))<<15;
        noise_shift>>=1;
    }
    if (length_counter[3]==0 || (noise_shift&1)) {
        noise_out = 0;
    } else {
        uint8_t noise_reg = cpu->memory[0x400C];
        uint8_t volume = noise_reg&0x10 ? noise_reg&0xf : env[2][2];
        noise_out = volume*(1-2*(noise_shift&1));
    }
    noise_timer++;
    noise_timer%=noise_periods[cpu->memory[0x400E]&0xf]/2;
}

void APU::start_sample() {
    current_address = sample_address;
    sample_bytes_remaining = sample_length;
}

void APU::dmc() {
    uint8_t r = dmc_flags&0xf;
    uint16_t current_period = dmc_period_table[r];

    //memory reader
    if (enabled[4] && sample_empty && sample_bytes_remaining!=0) {
        sample_buffer = cpu->memory[current_address];
        uint16_t bef = current_address;
        current_address++;
        if (current_address==0 && bef==0xffff) {
            current_address = 0x8000;
        }
        sample_bytes_remaining--;
        if (sample_bytes_remaining==0 && dmc_flags&0x40) {
            start_sample();
        }
        if (sample_bytes_remaining==0 && dmc_flags&0x80) {
            cpu->recv_irq = true;
        }
        if (sample_bytes_remaining>=0) {
            dmc_shift = sample_buffer;
            dmc_bits_remaining=8;
            dmc_silence = false;
            sample_empty = false;
        }
    }

    //dmc clock
    if (dmc_timer==0) {


        if (set_dmc==-1) {
            if (!dmc_silence) {
                if ((dmc_shift&1) && (dmc_out+2)<=127) {
                    dmc_out+=2;
                } else if (!(dmc_shift&1) && (dmc_out-2)>=0) {
                    dmc_out-=2;
                }
                dmc_shift>>=1;
                dmc_bits_remaining--;
                if (dmc_bits_remaining==0) {
                    dmc_bits_remaining=8;
                    if (sample_empty) {
                        dmc_silence = true;
                        dmc_out = 64;
                    } else {
                        dmc_silence = false;
                        sample_empty = true;
                    }
                }
            }
        } else {
            dmc_out = set_dmc;
            set_dmc = -1;
        }
    }


    dmc_timer++;
    dmc_timer%=current_period/2;
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