#ifndef UTIL_H
#define UTIL_H

#include <chrono>
#include <string>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "controller.h"

class CPU;
class Controller;

extern unsigned char out_img[184320]; //output image

//keys
extern const uint8_t* state;

//parameters
extern float global_volume;
extern float global_db;
extern bool use_shaders;
extern int changing_keybind;
extern SDL_Scancode mapped_keys[8];
//currently mapped keys - set to A,B,Select,Start,Up,Down,Left,Right.
//in the (reverse) order that the CPU reads at $4016

// vertices of quad covering entire screen with tex coords
extern GLfloat vertices[16];

//settings/pause menu
extern void pause_menu(void** system);
extern bool paused_window;

extern SDL_Joystick* controller;
        
int joystickDir(SDL_Joystick* joy);
#endif