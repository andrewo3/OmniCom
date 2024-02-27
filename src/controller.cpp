#include "controller.h"
#include <cstdint>
#include <cstdio>

Controller::Controller(bool* inputs)  {
    set_inputs(inputs);
}

uint8_t Controller::get_input_byte() {
    uint8_t res = 0;
    for (int i=0; i<8; i++) {
        res|=cont_inputs[7-i]<<i;
    }
    return res;
}