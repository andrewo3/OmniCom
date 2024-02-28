#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <cstdint>

class Controller {
    public:
        Controller(bool* inputs);
        void set_inputs(bool* new_inputs) {cont_inputs = new_inputs;}
        uint8_t get_input_byte();
        bool* cont_inputs;
        bool* A = &cont_inputs[0];
        bool* B = &cont_inputs[1];
        bool* Select = &cont_inputs[2];
        bool* Start = &cont_inputs[3];
        bool* Up = &cont_inputs[4];
        bool* Down = &cont_inputs[5];
        bool* Left = &cont_inputs[6];
        bool* Right = &cont_inputs[7];
};


#endif