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

#include "rom_data.h"
#include "rom.h"
#include "cpu.h"
#include "ppu.h"
#include "apu.h"
#include "util.h"
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


//ntsc filter options
static struct CRT crt;
static struct NTSC_SETTINGS ntsc;
static int color = 1;
static int noise = 6;
static int field = 0;
static int raw = 0;
static int hue = 0;

std::mutex interruptedMutex;

std::mutex audioBufferMutex;
int buffer_ind = 0;
int16_t* audio_buffer = new int16_t[BUFFER_LEN];

const int NES_DIM[2] = {256,240};
int WINDOW_INIT[2];
int filtered_res_scale = 2;
const int FLAGS = SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE;
float init_window_scale = 1/4.0;

volatile sig_atomic_t interrupted = 0;

long long total_ticks;
long long start;
long long start_nano;
double t = 0;
bool fullscreen_toggle = 0;
bool changed_use_shaders = false;
static int16_t audio_pt = 0;
volatile bool paused = false;
int diffs[10] = {0};
int frames = 0;
int desired_fps = 60;
long long paused_time = 0;
long long real_time = 0;
float t_time, last_time;

char* filename;
unsigned char * filtered;

GLuint shaderProgram;
GLuint vertexShader;
GLuint fragmentShader;

SDL_GLContext context;

GLuint texture;
GLuint VBO, VAO;

SDL_Window* window;

CPU *cpu_ptr;
PPU *ppu_ptr;
APU * apu_ptr;

ROM rom;

Controller* cont1;
Controller* cont2;

SDL_Event event;

char* device_name;
SDL_AudioSpec audio_spec;

SDL_AudioDeviceID audio_device;

std::thread tInputs;
std::thread sampleGet;
std::thread NESThread;

ImGuiIO io;


int usage_error() {
    printf("Usage: main rom_filename\n");
    return -1;
}

int invalid_error() {
    printf("Invalid NES rom!\n");
    return -1;
}

void quit(int signal) {
    std::lock_guard<std::mutex> lock(interruptedMutex);
    int clock_speed = cpu_ptr->emulated_clock_speed(real_time-paused_time);
    printf("Emulated Clock Speed: %li - Target: (approx.) 1789773 - %.02f%% similarity\n",clock_speed,clock_speed/1789773.0*100);
    //for test purpose: remove once done testing!!
    /*std::FILE* memory_dump = fopen("dump","w");
    fwrite(&cpu_ptr->memory[0x6004],sizeof(uint8_t),strlen((char*)(&cpu_ptr->memory[0x6004])),memory_dump);
    fclose(memory_dump);*/
    if (cpu_ptr->rom->battery_backed && !web) {
        std::FILE* ram_save = fopen((config_dir+sep+std::string("ram")).c_str(),"wb");
        cpu_ptr->save_ram(ram_save);
        fclose(ram_save);
    }
    interrupted = 1;
    if (signal==SIGSEGV) {
        printf("Segfault!\n");
        exit(EXIT_FAILURE);
    } else if (signal==SIGINT) {
        exit(0);
    }
}

void viewportBox(int** viewportBox,int width, int height) {
    float aspect_ratio = (float)NES_DIM[0]/NES_DIM[1];
    bool horiz = (float)width/height>aspect_ratio;
    (*viewportBox)[0] = horiz ? (width-aspect_ratio*height)/2.0 : 0;
    (*viewportBox)[1] = horiz ? 0 : (height-width/aspect_ratio)/2.0;
    (*viewportBox)[2] = horiz ? aspect_ratio*height : width;
    (*viewportBox)[3] = horiz ? height : width/aspect_ratio;
}

void init_shaders() {
    GLint success;
    GLchar infoLog[512];
    int vlen = web ? vertex_es_len : vertex_len;
    int flen = web ? fragment_es_len : fragment_len;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    char * vertex_source = new char[vlen+1];
    vertex_source[vlen] = 0;
    memcpy(vertex_source,web ? vertex_es : vertex,vlen);

    glShaderSource(vertexShader,1,&vertex_source, NULL);
    glCompileShader(vertexShader);

    GLint compileStatus;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    char * fragment_source = new char[flen+1];
    fragment_source[flen] = 0;
    memcpy(fragment_source,web ? fragment_es : fragment,flen);

    glShaderSource(fragmentShader,1,&fragment_source, NULL);
    glCompileShader(fragmentShader);

    //Check vertex shader compilation
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "Vertex Shader Compilation Failed:\n%s\n", infoLog);
    }

    // Check fragment shader compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "Fragment Shader Compilation Failed:\n%s\n", infoLog);
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "Shader Program Linking Failed:\n%s\n", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    delete[] vertex_source;
    delete[] fragment_source;
}

void CPUThread() {
    const double ns_wait = 1e9/cpu_ptr->CLOCK_SPEED;
    while(!interrupted) {
        cpu_ptr->clock();
        total_ticks = cpu_ptr->cycles;
        real_time = epoch_nano()-start_nano;
        long long cpu_time = ns_wait*cpu_ptr->cycles;
        int diff = cpu_time-(real_time-paused_time);
        if (diff > 0) {
            std::this_thread::sleep_for(std::chrono::nanoseconds(diff));
        }
    }
    
}

void PPUThread() {
    while(!interrupted) {
        if (ppu_ptr->cycles<cpu_ptr->cycles*3) {
            ppu_ptr->cycle();
            if (ppu_ptr->debug) {
                printf("PPU REGISTERS: ");
                printf("VBLANK: %i, PPUCTRL: %02x, PPUMASK: %02x, PPUSTATUS: %02x, OAMADDR: N/A (so far), PPUADDR: %04x\n",ppu_ptr->vblank, (uint8_t)cpu_ptr->memory[0x2000],(uint8_t)cpu_ptr->memory[0x2001],(uint8_t)cpu_ptr->memory[0x2002],ppu_ptr->v);
                printf("scanline: %i, cycle: %i\n",ppu_ptr->scanline,ppu_ptr->scycle);
            }
        }
    }
}

void APUThread() {
    while (apu_ptr->cycles*2<cpu_ptr->cycles) {
        apu_ptr->cycle();
        //apu_ptr->cycles++;
    }
}

void sampleAPU() {
    int sr = apu_ptr->sample_rate;
    const int ns_wait = (1e9/(sr*1));
    long long loops = 0;
    int16_t* buffer = new int16_t[BUFFER_LEN];
    int16_t* buffer_copy = new int16_t[BUFFER_LEN];
    long long last_q = 0;
    long long last_cycles = 0;
    long long cycles_per_audio = cpu_ptr->CLOCK_SPEED/sr;
    while (!interrupted) {
        apu_ptr->queue_mutex.lock();
        if (apu_ptr->queue_audio_flag) {
            int buffer_size = SDL_GetQueuedAudioSize(audio_device); 
            if (buffer_size>BUFFER_LEN*sizeof(int16_t)*4) { //clocked to run a little bit faster, so we must account for a slight overflow in samples - better than sending too little
                SDL_DequeueAudio(audio_device,nullptr,sizeof(int16_t)*BUFFER_LEN);
            } else {
                for (int i=0; i<BUFFER_LEN; i++) {
                    apu_ptr->buffer_copy[i]*=global_db;
                }

                SDL_QueueAudio(audio_device,apu_ptr->buffer_copy,sizeof(int16_t)*BUFFER_LEN);
            }
            //SDL_QueueAudio(audio_device,apu_ptr->buffer_copy,sizeof(int16_t)*BUFFER_LEN);
            apu_ptr->queue_audio_flag = false;
            apu_ptr->queue_mutex.unlock();
        }
    }
    delete[] buffer;
    delete[] buffer_copy;
}
//std::this_thread::sleep_for(std::chrono::milliseconds(500*BUFFER_LEN/sr));

void NESsequence(double ns_wait) {
    while (apu_ptr->cycles*2<cpu_ptr->cycles) {
        apu_ptr->cycle();
    }
    while (ppu_ptr->cycles<(cpu_ptr->cycles*3)) {
        ppu_ptr->cycle();
        
        
        if (ppu_ptr->debug) {
            printf("PPU REGISTERS: ");
            printf("VBLANK: %i, PPUCTRL: %02x, PPUMASK: %02x, PPUSTATUS: %02x, OAMADDR: N/A (so far), PPUADDR: %04x\n",ppu_ptr->vblank, (uint8_t)cpu_ptr->memory[0x2000],(uint8_t)cpu_ptr->memory[0x2001],(uint8_t)cpu_ptr->memory[0x2002],ppu_ptr->v);
            printf("scanline: %i, cycle: %i\n",ppu_ptr->scanline,ppu_ptr->scycle);
        }
    }
    cpu_ptr->clock();
    real_time = epoch_nano()-start_nano;
    long long cpu_time = ns_wait*cpu_ptr->cycles;
    int diff = cpu_time-(real_time-paused_time);
    if (cpu_time > (real_time-paused_time)) {
        std::this_thread::sleep_for(std::chrono::nanoseconds(diff));
    }
}

void NESLoop() {
    long ns_wait_l = (long)ns_wait;
    printf("Clock Speed: %i\n",cpu_ptr->CLOCK_SPEED);
    printf("Elapsed: %lf\n",ns_wait);
    void* system[3] = {cpu_ptr,ppu_ptr,apu_ptr};
    //emulator loop
    while (!interrupted) {
        if (!paused) {
            //if (clock_speed<=cpu_ptr->CLOCK_SPEED) { //limit clock speed
            //printf("clock speed: %i\n",cpu_ptr->emulated_clock_speed(real_time-paused_time));
            NESsequence(ns_wait);
        }
        
    }
    apu_ptr->queue_mutex.unlock();
    
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

void AudioLoop(void* userdata, uint8_t* stream, int len) {
    //printf("Buffer size: %i\n",len);
    std::lock_guard<std::mutex> lock(audioBufferMutex);
    for (int i=0; i<BUFFER_LEN; i++) {
        printf("%i,%f,%i\n",7000*(i==0),(double)frames/apu_ptr->sample_rate,audio_buffer[(buffer_ind+i)%BUFFER_LEN]);
        frames++;
    }
    int samplesToEnd = BUFFER_LEN - buffer_ind;
    memcpy(stream,&audio_buffer[buffer_ind],samplesToEnd*sizeof(int16_t));
    memcpy(stream+samplesToEnd*sizeof(int16_t),audio_buffer,buffer_ind*sizeof(int16_t));
}

void update_inputs() {
    //set up controller
    bool controller1_inputs[8];
    bool controller2_inputs[8];
    while (!interrupted) {
        //update controllers
        for (int i=0; i<8; i++) {
            if (current_device==0) {
                controller1_inputs[i] = state[mapped_keys[i]];
            } else {
                controller1_inputs[i] = mapped_joy[i]&0x80 ? ((SDL_JoystickGetHat(controller,0)&(mapped_joy[i]&0x7f)) || (joystickDir(controller)&(mapped_joy[i]&0x7f) && joystickDir(controller)!=-1)) 
                : SDL_JoystickGetButton(controller,mapped_joy[i]);
            }
            controller2_inputs[i] = false;
        }
        cont1->update_inputs(controller1_inputs);
        cont2->update_inputs(controller2_inputs);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/120));
    }
}

void mainLoop(void* arg) {
    //check if checkbox for shaders was clicked
    if (changed_use_shaders!=use_shaders) {
        GLfloat new_vertices[] = {
            -1.0f, 1.0f,0.0f,0.0f,
            -1.0f, -1.0f,0.0f,1-0.04f*use_shaders,
            1.0f, -1.0f,1.0f,1-0.04f*use_shaders,
            1.0f, 1.0f, 1.0f,0.0f
        };
        memcpy(vertices,new_vertices,16*sizeof(GLfloat));
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, use_shaders ? GL_LINEAR : GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, use_shaders ? GL_LINEAR : GL_NEAREST);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);
    }
    changed_use_shaders = use_shaders;
    //logic is executed in nes thread
    if (!paused) {
        ppu_ptr->image_mutex.lock();
    }
    if (!ppu_ptr->image_drawn || paused) { //if ppu hasnt registered image as being drawn yet
        t_time = SDL_GetTicks()/1000.0;
        float diff = t_time-last_time;
        #ifndef __EMSCRIPTEN__
            char * new_title = new char[255];
            sprintf(new_title,"%s - %.02f FPS",filename,1/diff);
            SDL_SetWindowTitle(window,new_title);
            delete[] new_title;
        #endif
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        

        last_time = t_time;
    
        if (use_shaders) {
            //apply ntsc filter before drawing
            ntsc.data = ppu_ptr->getImg(); /* buffer from your rendering */
            ntsc.format = CRT_PIX_FORMAT_RGB;
            ntsc.w = NES_DIM[0];
            ntsc.h = NES_DIM[1];
            ntsc.as_color = color;
            ntsc.field = field & 1;
            ntsc.raw = raw;
            ntsc.hue = hue;
            if (ntsc.field == 0) {
            ntsc.frame ^= 1;
            }
            crt_modulate(&crt, &ntsc);
            crt_demodulate(&crt, noise);
            field ^= 1;
            //render texture from nes (temporarily test_image.jpg)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, NES_DIM[0]*filtered_res_scale,NES_DIM[1]*filtered_res_scale, 0, GL_RGB, GL_UNSIGNED_BYTE, filtered);
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, NES_DIM[0],NES_DIM[1], 0, GL_RGB, GL_UNSIGNED_BYTE, ppu_ptr->getImg());

        }
        glUseProgram(shaderProgram);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN,0,4);
        glBindVertexArray(0);
        glUseProgram(0);

        //ImGui::ShowDemoWindow(NULL);
        //render gui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();
        if (paused) {
            SDL_Delay(1000/desired_fps);
            void * system[3] = {cpu_ptr,ppu_ptr,apu_ptr};
            pause_menu(&system[0]);
        } else {
            changing_keybind = -1;
        }
        if (paused && !paused_window) {
            paused_time += (epoch_nano()-start_nano)-real_time;
            paused = false;              
        }
        ImGui::Render();
        
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(window,context);
        }
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //reset viewport
        int* new_viewport = new int[4];
        SDL_GetWindowSize(window,&new_viewport[2],&new_viewport[3]);
        viewportBox(&new_viewport,new_viewport[2],new_viewport[3]);
        glViewport(new_viewport[0],new_viewport[1],new_viewport[2],new_viewport[3]);
        delete[] new_viewport;

        //update screen
        SDL_GL_SwapWindow(window);
        
        ppu_ptr->image_mutex.unlock();
        ppu_ptr->image_drawn = true;
    

        //ppu_ptr->image_mutex.unlock();
        // event loop
        #ifdef __EMSCRIPTEN__
        if (display_size_changed)
        {
            double w, h;
            emscripten_get_element_css_size( "#canvas", &w, &h );
            SDL_SetWindowSize( window, (int)w, (int) h );

            display_size_changed = 0;
        }
        #endif
        SDL_PumpEvents();
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    quit(0);
                    break;
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        int* new_viewport = new int[4];
                        int new_width = event.window.data1;
                        int new_height = event.window.data2;
                        viewportBox(&new_viewport,new_width,new_height);
                        printf("%i %i %i %i\n",new_viewport[0],new_viewport[1],new_viewport[2],new_viewport[3]);
                        glViewport(new_viewport[0],new_viewport[1],new_viewport[2],new_viewport[3]);
                        //glViewport(0,0,new_width,new_height);
                        delete[] new_viewport;
                    } else if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                        quit(0);
                        break;
                    }
                    break;
                case SDL_KEYDOWN:
                    if (changing_keybind>-1 && current_device == 0) {
                        mapped_keys[changing_keybind] = event.key.keysym.scancode;
                        changing_keybind = -1;
                    }
                    switch (event.key.keysym.sym) {
                        case SDLK_c:
                            {
                            SDL_Scancode modifier = SDL_SCANCODE_LCTRL;
                            #ifdef __APPLE__
                                modifier = SDL_SCANCODE_LGUI;
                            #endif
                            if (state[modifier] && !paused) {
                                printf("Current Emulated Clock Speed: %i\n",cpu_ptr->emulated_clock_speed(real_time-paused_time));
                            }
                            break;
                            }
                        #ifndef __EMSCRIPTEN__
                        case SDLK_F11:
                            {
                            fullscreen_toggle = fullscreen_toggle ? false : true;
                            SDL_SetWindowFullscreen(window,fullscreen_toggle*SDL_WINDOW_FULLSCREEN_DESKTOP);
                            SDL_SetWindowSize(window,WINDOW_INIT[0]/2,WINDOW_INIT[1]/2);
                            SDL_SetWindowPosition(window,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED);
                            int* new_viewport = new int[4];
                            viewportBox(&new_viewport,WINDOW_INIT[0]/2,WINDOW_INIT[1]/2);
                            printf("%i %i %i %i\n",new_viewport[0],new_viewport[1],new_viewport[2],new_viewport[3]);
                            glViewport(new_viewport[0],new_viewport[1],new_viewport[2],new_viewport[3]);
                            delete[] new_viewport;
                            break;
                            }
                        #endif
                        case SDLK_r:
                            {
                            SDL_Scancode modifier = SDL_SCANCODE_LCTRL;
                            #ifdef __APPLE__
                                modifier = SDL_SCANCODE_LGUI;
                            #endif
                            
                            if (state[modifier] && !paused) { //ctrl+r (or cmd+r) - reset shortcut
                                interrupted = true;
                                sampleGet.join();
                                NESThread.join();
                                tInputs.join();
                                //tCPU.join();
                                //tPPU.join();
                                //tAPU.join();
                                cpu_ptr->init_vals();
                                rom.reset_mapper();
                                cpu_ptr->loadRom(&rom);
                                ppu_ptr->loadRom(&rom);
                                cpu_ptr->reset();

                                interrupted = false;
                                NESThread = std::thread(NESLoop);
                                tInputs = std::thread(update_inputs);
                                //tCPU = std::thread(CPUThread);
                                //tPPU = std::thread(PPUThread);
                                //tAPU = std::thread(APUThread);
                                sampleGet = std::thread(sampleAPU);
                            }
                            break;
                            }
                        case SDLK_ESCAPE:
                            paused = paused ? false : true;
                            paused_window = true;
                            if (!paused) {
                                paused_time += (epoch_nano()-start_nano)-real_time;
                            }
                            cpu_ptr->last = epoch_nano(); // reset timing
                            break;
                    }
                    break;
                case SDL_JOYBUTTONDOWN:
                    if (changing_keybind>-1 && current_device > 0) {
                        mapped_joy[changing_keybind] = event.jbutton.button;
                        changing_keybind = -1;
                    }
                    break;
                case SDL_JOYHATMOTION:
                    if (changing_keybind>-1 && current_device > 0) {
                        if (event.jhat.value == SDL_HAT_LEFT || event.jhat.value == SDL_HAT_RIGHT
                        || event.jhat.value == SDL_HAT_UP || event.jhat.value == SDL_HAT_DOWN) {
                            mapped_joy[changing_keybind] = event.jhat.value|0x80;
                            changing_keybind = -1;
                        }
                    }
                    break;
                case SDL_JOYAXISMOTION:
                    if (changing_keybind>-1 && current_device > 0) {
                        if (joystickDir(controller) != -1) {
                            mapped_joy[changing_keybind] = joystickDir(controller)|0x80;
                            changing_keybind = -1;
                        }
                    }
                    break;
            }
            ImGui_ImplSDL2_ProcessEvent(&event);
        }
    }
}

int main(int argc, char ** argv) {
    std::signal(SIGINT,quit);
    std::signal(SIGSEGV,quit);
    web = WEB;
    int os = -1;

    #ifdef __APPLE__
        config_dir = std::string(std::getenv("HOME"))+"/Library/Containers";
        sep = '/';
        printf("MACOS, %s\n", config_dir.c_str());
        os = 0;
    #endif
    #ifdef __WIN32__
        TCHAR appdata[MAX_PATH] = {0};
        SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, appdata);
        config_dir = std::string(appdata);
        sep = '\\';
        printf("WINDOWS, %s\n", config_dir.c_str());
        os = 1;
    #endif
    #ifdef __unix__
        config_dir = std::string(std::getenv("HOME"))+"/.config";
        sep = '/';
        printf("LINUX, %s\n", config_dir.c_str());
        os = 2;
    #endif

    if (argc!=ARGS) {
        return usage_error();
    }
    if (ARGS==2) { //nothing specified to replace at compile time
        rom.load_file(ROM_NAME);
        if (!rom.is_valid()) {
            return invalid_error();
        }
    } else {
        unsigned char* placeholder; // if DATAROM not defined - should never actually be used.
        rom.load_arr(DATALENGTH,DATAROM);
        if (!rom.is_valid()) {
            return invalid_error();
        }
    }

    printf("Mapper: %i\n",rom.get_mapper()->type); //https://www.nesdev.org/wiki/Mapper
    printf("Mirrormode: %i\n",rom.mirrormode);

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

    //audio
    audio_spec.freq = 44100;
    audio_spec.format = AUDIO_S16SYS;  // 16-bit signed, little-endian
    audio_spec.channels = 1;            // Mono
    audio_spec.samples = BUFFER_LEN;
    audio_spec.size = BUFFER_LEN * sizeof(int16_t) * audio_spec.channels;
    //audio_spec.callback = AudioLoop;
    audio_device = SDL_OpenAudioDevice(device_name,0,&audio_spec,nullptr,0);
    //stream = SDL_NewAudioStream(AUDIO_U8,1,44100,);
    printf("SDL audio set up.\n");

    //GL Context and Window Init
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0"); // for linux
    
    printf("GL attributes\n");
    if (!web) {
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        printf("GL Major version: 3\n");
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 2);
        printf("GL Minor version: 2\n");
    } else {
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES );
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        printf("GL ES Major version: 2\n");
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 0);
        printf("GL ES Minor version: 0\n");
    }
    int* viewport = new int[4];
    viewportBox(&viewport,WINDOW_INIT[0],WINDOW_INIT[1]);
    printf("Viewport set\n");
    printf("%i %i %i %i\n",viewport[0],viewport[1],viewport[2],viewport[3]);
    window = SDL_CreateWindow(filename,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WINDOW_INIT[0],WINDOW_INIT[1],FLAGS);
    printf("Made window\n");

    SDL_SetWindowTitle(window,filename);
    printf("Window Title Set\n");
    context = SDL_GL_CreateContext(window);
    if (!context) {
        SDL_Log("Failed to create OpenGL context: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    glewExperimental = GL_TRUE;
    glewInit();
    glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);
    //SDL_GL_SetSwapInterval(0);
    if (os == 0) { // Mac

    } else if (os == 1) { // Windows
        /*typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALEXTPROC)(int interval);
        PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT");  
        wglSwapIntervalEXT(1);*/
    } else if (os==2) { // Linux

    }
    printf("OpenGL Initialized.\n");
    GLenum error = glGetError();
    

    //Dear Imgui init
    ImGui::CreateContext();
    #ifndef __EMSCRIPTEN__
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    #endif
    io = ImGui::GetIO();
    ImGui_ImplSDL2_InitForOpenGL(window,context);
    #ifdef __EMSCRIPTEN__
        ImGui_ImplOpenGL3_Init("#version 100");
    #else
        ImGui_ImplOpenGL3_Init("#version 330 core");
    #endif
    // Shader init
    init_shaders();
    printf("Shaders compiled and linked.\n");
    //ntsc filter init

    /* pass it the buffer to be drawn on screen */
    filtered = (unsigned char*)malloc(NES_DIM[0]*NES_DIM[1]*filtered_res_scale*filtered_res_scale*3*sizeof(char));
    crt_init(&crt, NES_DIM[0]*filtered_res_scale,NES_DIM[1]*filtered_res_scale, CRT_PIX_FORMAT_RGB, &filtered[0]);
    /* specify some settings */
    crt.blend = 0;
    crt.scanlines = 1;


    //VBO & VAO init for the fullscreen texture
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //texture init
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, NES_DIM[0],NES_DIM[1], 0, GL_RGB, GL_UNSIGNED_BYTE, out_img);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //glGenerateMipmap(GL_TEXTURE_2D);

    glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"), 0);
    
    printf("Window texture bound and mapped.\n");

    //set up controller
    bool controller1_inputs[8];
    bool controller2_inputs[8];
    for (int i=0; i<8; i++) {
        controller1_inputs[i] = state[mapped_keys[i]];
        controller2_inputs[i] = false;
    }
    cont1 = new Controller(controller1_inputs);
    cont2 = new Controller(controller2_inputs);
    printf("Virtual Controller Objects initialized.\n");
    tInputs = std::thread(update_inputs);
    printf("Input Thread started.\n");
    start = epoch();
    start_nano = epoch_nano();

    cpu_ptr = new CPU(false);
    printf("CPU Initialized.\n");
    static APU apu;
    apu.sample_rate = audio_spec.freq;
    cpu_ptr->apu = &apu;
    apu_ptr = &apu;
    apu.setCPU(cpu_ptr);
    printf("APU set\n");

    cpu_ptr->loadRom(&rom,!((bool)web));
    cpu_ptr->set_controller(cont1,0);
    cpu_ptr->set_controller(cont2,1);
    cpu_ptr->reset();
    printf("ROM loaded into CPU.\n");

    ns_wait = 1e9/cpu_ptr->CLOCK_SPEED;

    ppu_ptr = new PPU(cpu_ptr);
    ppu_ptr->debug = false;
    printf("PPU Initialized\n");
    load = false;
    if (load && !web) {
        FILE* save_file = fopen((config_dir+sep+std::string("state")).c_str(),"rb");
        cpu_ptr->load_state(save_file);
        fclose(save_file);
        printf("Loaded save\n");
    }

    //Enter NES logic loop alongside window loop
    NESThread = std::thread(NESLoop);
    sampleGet = std::thread(sampleAPU);
    //std::thread tCPU(CPUThread);
    //std::thread tPPU(PPUThread);
    //std::thread tAPU(APUThread);

    printf("NES thread started. Starting main window loop...\n");
    printf("x,y\n");
    t_time = SDL_GetTicks()/1000.0;
    last_time = SDL_GetTicks()/1000.0;
    int16_t buffer[BUFFER_LEN*2];
    SDL_PauseAudioDevice(audio_device,0);
    //main window loop
    #ifdef __EMSCRIPTEN__
        int fps = 0;
        emscripten_set_resize_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW,
        0, 0, on_web_display_size_changed
        );
        emscripten_set_main_loop_arg(mainLoop,(void*)nullptr,fps,true);
    #else
        while (!interrupted) {
            mainLoop((void*)nullptr);
        }
    #endif
    tInputs.join();
    NESThread.join();
    sampleGet.join();
    glDetachShader(shaderProgram,vertexShader);
    glDetachShader(shaderProgram,fragmentShader);
    glDeleteProgram(shaderProgram);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_CloseAudioDevice(audio_device);
    SDL_Quit();
    free(filtered);
    //delete cont1;
    delete[] original_start;
    delete[] audio_buffer;
    delete cont1;
    delete cont2;
    delete ppu_ptr;
    delete cpu_ptr;
    printf("Quit successfully.\n");
    //tCPU.join();
    //tPPU.join();
    //tAPU.join();
    //stbi_image_free(img);
    return 0;
}