#ifndef UTIL_H
#define UTIL_H

#include <chrono>
#include <string>
#include <SDL2/SDL.h>
#ifndef WEB
#include <GL/glew.h>
#else
#include <GLES3/gl3.h>
#endif
#include "crt_core.h"

class CPU;
class Controller;
class BaseSystem;

extern unsigned char out_img[184320]; //output image

//keys
extern const uint8_t* state;

//parameters
extern float global_volume;
extern float global_db;
extern bool use_shaders;
extern int changing_keybind;
extern SDL_Scancode mapped_keys[8];
extern uint8_t mapped_joy[8];
extern int current_device;
//currently mapped keys - set to A,B,Select,Start,Up,Down,Left,Right.
//in the (reverse) order that the CPU reads at $4016

// vertices of quad covering entire screen with tex coords
extern GLfloat vertices[16];

//settings/pause menu
extern void pause_menu(BaseSystem* saveSystem);
extern bool paused_window;
extern volatile bool paused;

extern SDL_Joystick* controller;
        
int joystickDir(SDL_Joystick* joy);

void setGLViewport(int width, int height, float aspect_ratio);

//ntsc filter options
static struct CRT crt;
static struct NTSC_SETTINGS ntsc;
static int color = 1;
static int noise = 6;
static int field = 0;
static int raw = 0;
static int hue = 0;

int default_config();
#endif