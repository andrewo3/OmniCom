#include "window/window.h"
#include "GL/glew.h"
#include "util.h"
#include "glob_const.h"
#include <string>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

const int FLAGS = SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE;

EmuWindow::EmuWindow(std::string title,int w, int h) {
    win = SDL_CreateWindow(title.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,w,h,FLAGS);
    printf("Made window: %p\n",win);
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

void EmuWindow::setupAudio() {
    //audio
    SDL_zero(audio_spec);
    audio_spec.freq = 44100;
    audio_spec.format = AUDIO_S16SYS;  // 16-bit signed, little-endian
    audio_spec.channels = 1;            // Mono
    audio_spec.samples = BUFFER_LEN;
    audio_spec.size = BUFFER_LEN * sizeof(int16_t) * audio_spec.channels;
    //audio_spec.callback = AudioLoop;
    SDL_AudioSpec obtained;
    char* device_name;
    int success = SDL_GetDefaultAudioInfo(&device_name,&obtained,0);
    printf("Obtained device: %s\n",device_name);
    audio_device = SDL_OpenAudioDevice(device_name,0,&audio_spec,nullptr,0);
    if (audio_device != 0) {
        const char* audio_device_name = SDL_GetAudioDeviceName(audio_device,0);
        printf("Audio Device: %s\n",audio_device_name);
        SDL_PauseAudioDevice(audio_device,0);
        SDL_free(device_name);
    } else {
        printf("Audio Device: FAILURE\n");
        SDL_DestroyWindow(win);
        SDL_Quit();
        window_created = false;
    }
    
}

void EmuWindow::Close() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_CloseAudioDevice(audio_device);
}

void EmuWindow::ImGuiInit() {
    //Dear Imgui init
    ImGui::CreateContext();
    #ifndef __EMSCRIPTEN__
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    #endif
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    io = ImGui::GetIO();
    ImGui_ImplSDL2_InitForOpenGL(GetSDLWin(),GetGLContext());
    #ifdef __EMSCRIPTEN__
        ImGui_ImplOpenGL3_Init("#version 100");
    #else
        ImGui_ImplOpenGL3_Init("#version 330 core");
    #endif
}

void EmuWindow::drawPauseMenu(BaseSystem* saveSystem) {
    //render gui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(GetSDLWin());
    ImGui::NewFrame();
    if (paused) {
        SDL_Delay(1000/desired_fps);
        pause_menu(saveSystem);
    } else {
        changing_keybind = -1;
    }
    if (paused && !paused_window) {
        paused = false;              
    }
    ImGui::Render();
    
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(GetSDLWin(),GetGLContext());
    }
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

EmuWindow::~EmuWindow() {
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(win);
}

void EmuWindow::setTitle(std::string new_title) {
    SDL_SetWindowTitle(win,new_title.c_str());
}