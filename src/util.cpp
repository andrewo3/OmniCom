#include "util.h"
#include "SDL2/SDL.h"
#include "math.h"

const int BUFFER_LEN = 512;
unsigned char out_img[184320]; //output image
const uint8_t* state = SDL_GetKeyboardState(nullptr);
SDL_Joystick* controller = NULL;

int joystickDir(SDL_Joystick* joy) {
    float x = SDL_JoystickGetAxis(joy,0)/32768.0;
    float y = SDL_JoystickGetAxis(joy,1)/32768.0;
    if (x>y && x>sqrt(2)/2) {
        return 0; //right
    } else if (x>y && y<-sqrt(2)/2) {
        return 3; //up
    } else if (x<y && x<-sqrt(2)/2) {
        return 2; //left
    } else if (x<y && y>sqrt(2)/2) {
        return 1; //down
    }
    return -1;
}