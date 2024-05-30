#include "controller.h"
#include <cstdint>
#include <cstdbool>
#include <cstdio>
#include <cstring>

using namespace NES;

Controller::Controller(bool* inputs)  {
    update_inputs(inputs);
}

void Controller::update_inputs(bool* new_inputs) {
    memcpy(cont_inputs,new_inputs,8*sizeof(bool));
    A = cont_inputs[0];
    B = cont_inputs[1];
    Select = cont_inputs[2];
    Start = cont_inputs[3];
    Up = cont_inputs[4];
    Down = cont_inputs[5];
    Left = cont_inputs[6];
    Right = cont_inputs[7];
}

uint8_t Controller::get_input_byte() {
    uint8_t res = 0;
    for (int i=0; i<8; i++) {
        res|=(cont_inputs[7-i])<<i;
    }
    return res;
}