#ifndef window_h
#define window_h
#include "SDL2/SDL.h"
#include "GL/glew.h"
#include <string>
#include <stdbool.h>


class EmuWindow {
    public:
        EmuWindow(std::string title, int w, int h);
        ~EmuWindow();
        void setTitle(std::string new_title);
        bool window_created;
        SDL_Window* win;
        SDL_GLContext context;
        SDL_Window* GetSDLWin() {
            return win;
        }
        SDL_GLContext GetGLContext() {
            return context;
        }
};

#endif