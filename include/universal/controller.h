#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <cstdint>

class Controller {
    public:
        Controller(bool* inputs);
        void set_inputs(bool* new_inputs) {cont_inputs = new_inputs;}
        uint8_t get_input_byte();
        bool* cont_inputs;
};


#endif