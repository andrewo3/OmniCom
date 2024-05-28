#include "window/window.h"
#include "GL/glew.h"
#include <string>

const int FLAGS = SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE;

EmuWindow::EmuWindow(std::string title,int w, int h) {
    win = SDL_CreateWindow(title.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,w,h,FLAGS);
    printf("Made window\n");
    context = SDL_GL_CreateContext(win);
    if (!context) {
        SDL_Log("Failed to create OpenGL context: %s", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        window_created = false;
    }
    glewExperimental = GL_TRUE;
    glewInit();
    window_created = true;
}

EmuWindow::~EmuWindow() {
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(win);
}

void EmuWindow::setTitle(std::string new_title) {
    SDL_SetWindowTitle(win,new_title.c_str());
}