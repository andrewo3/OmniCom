#include "util.h"
#include "SDL2/SDL.h"

unsigned char out_img[184320]; //output image
const uint8_t* state = SDL_GetKeyboardState(nullptr);
SDL_Joystick* controller = NULL;