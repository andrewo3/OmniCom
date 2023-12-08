#include "util.h"
#include "SDL2/SDL.h"
#include <queue>

unsigned char out_img[184320]; //output image
std::queue<int16_t> audio_buffer; //output audio stream
const uint8_t* state = SDL_GetKeyboardState(nullptr);