#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <cstdint>
#include <cstdbool>

class Controller {
    public:
        Controller() {}
        Controller(bool* inputs);
        void update_inputs(bool* new_inputs);
        uint8_t get_input_byte();
        bool cont_inputs[8] = {0};
        bool A;
        bool B;
        bool Select;
        bool Start;
        bool Up;
        bool Down;
        bool Left;
        bool Right;
};


#endif