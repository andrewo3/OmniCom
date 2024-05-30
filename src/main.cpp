#define ARGS 1
#ifndef ROM_NAME
    #define ROM_NAME argv[1]
    #define ARGS 2
#endif
#ifndef DATAROM
    #define DATAROM placeholder
    #define ARGS 2
#endif
#ifndef DATALENGTH
    #define DATALENGTH 0
#endif
#ifndef WEB
    #define WEB 0
#endif
#ifndef ROMCONSOLE
    #define ROMCONSOLE CONSOLE::NES_rom
#endif

#define SDL_MAIN_HANDLED
#ifdef __WIN32__
#include "Shlobj.h"
#include "Windows.h"
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include "SDL2/SDL.h"
#include "GL/glew.h"

#include "nes_sys.h"
#include "rom_data.h"
#include "util.h"
#include "window/window.h"
#include "controller.h"
#include "shader_data.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include "crt_core.h"

#include <cstdio>
#include <string>
#include <csignal>
#include <chrono>
#include <cmath>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <filesystem>
#include <cstdlib>

std::mutex interruptedMutex;

std::mutex audioBufferMutex;
int buffer_ind = 0;
int16_t* audio_buffer = new int16_t[BUFFER_LEN];

int WINDOW_INIT[2];
const int FLAGS = SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE;
float init_window_scale = 1/4.0;

volatile sig_atomic_t interrupted = 0;

uint8_t* rom_data = NULL;
long rom_len;

long long total_ticks;
long long start;
long long start_nano;
double t = 0;
bool fullscreen_toggle = 0;
bool changed_use_shaders = false;
static int16_t audio_pt = 0;
int diffs[10] = {0};
int frames = 0;
int desired_fps = 60;
long long paused_time = 0;
long long real_time = 0;
float t_time, last_time;

char* filename;
unsigned char * filtered;

SDL_GLContext context;

GLuint texture;
GLuint VBO, VAO;

EmuWindow* window;
BaseSystem* emuSystem;

SDL_Event event;


int usage_error() {
    printf("Usage: main rom_filename\n");
    return -1;
}

int invalid_error() {
    printf("Invalid NES rom!\n");
    return -1;
}

void quit(int signal) {
    //for test purpose: remove once done testing!!
    /*std::FILE* memory_dump = fopen("dump","w");
    fwrite(&cpu_ptr->memory[0x6004],sizeof(uint8_t),strlen((char*)(&cpu_ptr->memory[0x6004])),memory_dump);
    fclose(memory_dump);*/
    emuSystem->Stop();
    emuSystem->running = 0;
    if (signal==SIGSEGV) {
        printf("Segfault!\n");
        exit(EXIT_FAILURE);
    } else if (signal==SIGINT) {
        printf("Interrupted.\n");
        exit(0);
    }
}


#ifdef __EMSCRIPTEN__
int display_size_changed = 0;

static EM_BOOL on_web_display_size_changed( int event_type, 
const EmscriptenUiEvent *event, void *user_data )
{
    display_size_changed = 1;  // custom global flag
    return 0;
}

#endif

int setRomData(std::string filename) {
    FILE* fp = fopen(filename.c_str(),"rb");
    if (fp!=NULL) {
        fseek(fp,0,SEEK_END);
        rom_len = ftell(fp);
        fseek(fp,0,SEEK_SET);
        
        rom_data = (uint8_t*)malloc(sizeof(uint8_t)*rom_len);
        fread(rom_data,sizeof(uint8_t),rom_len,fp);
        fclose(fp);
        return 0;
    } else {
        return 1;
    }
}

void mainLoop(void* arg) {
    //logic is executed in nes thread
    bool new_frame = emuSystem->Render(); //Execute GL render functions from system
    SDL_ShowCursor(paused);
    if (paused) {
        window->drawPauseMenu(emuSystem);
    }

    //update screen
    if (new_frame || paused) {
        SDL_GL_SwapWindow(window->GetSDLWin());
    }
    /*if (audio_queue.size() >= BUFFER_LEN) {
        SDL_QueueAudio(audio_device,audio_queue.data(),sizeof(int16_t)*audio_queue.size());
        audio_queue.clear();
    }*/
    


    //ppu_ptr->image_mutex.unlock();
    // event loop
    #ifdef __EMSCRIPTEN__
    if (display_size_changed)
    {
        double w, h;
        emscripten_get_element_css_size( "#canvas", &w, &h );
        SDL_SetWindowSize( window->GetSDLWin(), (int)w, (int) h );

        display_size_changed = 0;
    }
    #endif
    //process events
    emuSystem->Update();
}

int main(int argc, char ** argv) {
    emuSystem = new NES::System();
    std::signal(SIGINT,quit);
    std::signal(SIGSEGV,quit);
    web = WEB;

    //set "config_dir" to system directory, and set os variable accordingly
    int os = default_config();
    if (argc!=ARGS) {
        return usage_error();
    }

    if (ARGS==2) { //if nothing specified to replace at compile time
        if (setRomData((std::string(ROM_NAME)))) {
            return invalid_error();
        }
    } else {
        uint8_t* placeholder = NULL;
        rom_data = (uint8_t*)malloc(sizeof(uint8_t)*DATALENGTH);
        memcpy(rom_data,DATAROM,sizeof(uint8_t)*DATALENGTH);
        rom_len = DATALENGTH;
    }
    filename = new char[strlen(ROM_NAME)+1];
    char* original_start = filename;
    memcpy(filename,ROM_NAME,strlen(ROM_NAME)+1);
    get_filename(&filename);

    char removed_spaces[strlen(filename)+1];

    for (int i=0; i<strlen(filename); i++) {
        removed_spaces[i] = filename[i];
        if (removed_spaces[i]==' ') {
            removed_spaces[i] = '_';
        }
    }
    removed_spaces[strlen(filename)] = '\0';

    //make config dir (if it doesnt already exist)
    bool load = false;
    printf("Web: %i\n",web);
    if (os != -1 && !web) {
        config_dir+=sep;
        config_dir+=std::string("Nes2Exec");
        if (!std::filesystem::exists(config_dir)) { //make Nes2Exec appdata folder
            std::filesystem::create_directory(config_dir);
        }
        config_dir+=sep;
        config_dir+=std::string(removed_spaces);
        printf("%s\n",(config_dir).c_str());
        if (!std::filesystem::exists(config_dir)) { //make specific game save folder
            std::filesystem::create_directory(config_dir);
        } else {
            printf("Folder already exists. Checking for save...\n");
            if (std::filesystem::exists(config_dir+sep+std::string("state"))) {
                load = true;
            }
        }
    } else {
        printf("OS not detected. No save folder created.\n");
    }

    // SDL initialize
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK);
    //SDL_Vulkan_LoadLibrary(nullptr);
    SDL_ShowCursor(0);
    printf("SDL Initialized\n");

    //set display dimensions
    SDL_DisplayMode DM;
    SDL_GetDesktopDisplayMode(0,&DM);
    WINDOW_INIT[0] = DM.w*init_window_scale;
    WINDOW_INIT[1] = DM.h*init_window_scale;

    //stream = SDL_NewAudioStream(AUDIO_U8,1,44100,);
    printf("SDL audio set up.\n");

    //GL Context and Window Init
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0"); // for linux
    
    printf("GL attributes\n");
    #ifndef __EMSCRIPTEN__
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    printf("GL Major version: 3\n");
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 2);
    printf("GL Minor version: 2\n");
    #else
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES );
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    printf("GL ES Major version: 2\n");
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 0);
    printf("GL ES Minor version: 0\n");
    #endif
    window = new EmuWindow((std::string(filename)),WINDOW_INIT[0],WINDOW_INIT[1]);
    emuSystem->setWindow(window);
    emuSystem->GLSetup();
    window->ImGuiInit();
    window->setupAudio();
    if (!window->window_created) {
        return 1;
    }
    setGLViewport(WINDOW_INIT[0], WINDOW_INIT[1], (float)emuSystem->video_dim[0]/emuSystem->video_dim[1]);
    printf("OpenGL Initialized.\n");
    GLenum error = glGetError();


    //start system
    emuSystem->loadRom(rom_len,rom_data);
    free(rom_data);
    printf("Rom loaded.\n");

    //Enter NES logic loop alongside window loop
    emuSystem->Start();
    printf("Emulator system started - running: %i\n",emuSystem->running);

    printf("x,y\n");
    t_time = SDL_GetTicks()/1000.0;
    last_time = SDL_GetTicks()/1000.0;
    int16_t buffer[BUFFER_LEN*2];
    //main window loop
    #ifdef __EMSCRIPTEN__
        int fps = 0;
        emscripten_set_resize_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW,
        0, 0, on_web_display_size_changed
        );
        emscripten_set_main_loop_arg(mainLoop,(void*)nullptr,fps,true);
    #else
        while (emuSystem->running) {
            mainLoop((void*)nullptr);
        }
    #endif
    emuSystem->Stop();
    window->Close();
    SDL_Quit();
    free(filtered);
    //delete cont1;
    delete[] original_start;
    delete[] audio_buffer;
    delete window;
    printf("Quit successfully.\n");
    //tCPU.join();
    //tPPU.join();
    //tAPU.join();
    //stbi_image_free(img);
    return 0;
}