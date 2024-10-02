#ifndef window_h
#define window_h
#include "SDL2/SDL.h"
#ifndef WEB
#include "GL/glew.h"
#else
#include <GLES3/gl3.h>
#endif
#include <string>
#include <stdbool.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "system.h"

class BaseSystem;

class EmuWindow {
    public:
        EmuWindow(std::string title, int w, int h);
        ~EmuWindow();
        void setTitle(std::string new_title);
        void ImGuiInit();
        void setupAudio();
        void Close();
        void drawPauseMenu(BaseSystem* saveSystem);
        volatile bool window_created;
        int desired_fps = 60;
        volatile SDL_AudioDeviceID audio_device;
        SDL_Window* win = NULL;
        SDL_GLContext context;
        ImGuiIO io;
        SDL_Window* GetSDLWin() {
            return win;
        }
        SDL_GLContext GetGLContext() {
            return context;
        }
        SDL_AudioSpec getAudioSpec() {
            return audio_spec;
        }
    private:
        SDL_AudioSpec audio_spec;
};

#endif