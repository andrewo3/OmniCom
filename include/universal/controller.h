#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <cstdint>
#include <cstdbool>

class Controller {
    public:
        Controller(bool** inputs);
        void set_inputs(bool** new_inputs);
        uint8_t get_input_byte();
        bool** cont_inputs;
        bool* A;
        bool* B;
        bool* Select;
        bool* Start;
        bool* Up;
        bool* Down;
        bool* Left;
        bool* Right;
};


#endif