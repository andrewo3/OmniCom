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

#define SDL_MAIN_HANDLED
#ifdef __WIN32__
#include "Shlobj.h"
#include "Windows.h"
#endif
#include "SDL2/SDL.h"
#include "GL/glew.h"

#include "SDL2/SDL_vulkan.h"
#include "vulkan/vulkan.hpp"

#include "rom_data.h"
#include "rom.h"
#include "cpu.h"
#include "ppu.h"
#include "apu.h"
#include "util.h"
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
static int16_t audio_pt = 0;
bool paused = false;
long clock_speed = 0;
int diffs[10] = {0};
int frames = 0;
int desired_fps = 60;
long long paused_time = 0;
long long real_time = 0;

GLuint shaderProgram;
GLuint vertexShader;
GLuint fragmentShader;

CPU *cpu_ptr;
PPU *ppu_ptr;
APU * apu_ptr;
SDL_AudioDeviceID audio_device;
char* device_name;
SDL_AudioSpec audio_spec;



int usage_error() {
    printf("Usage: main rom_filename\n");
    return -1;
}

int invalid_error() {
    printf("Invalid NES rom!\n");
    return -1;
}

void get_filename(char** path) {
    int l = strlen(*path);
    bool end_set = false;
    for (int i=l-1; i>=0; i--) {
        if ((*path)[i]=='.' && !end_set) {
            (*path)[i] = '\0';
            end_set = true;
        } else if ((*path)[i]=='/' || (*path)[i]=='\\') {
            *path = &(*path)[i+1];
            return;
        }
    }
}

void quit(int signal) {
    std::lock_guard<std::mutex> lock(interruptedMutex);
    printf("Emulated Clock Speed: %li - Target: (approx.) 1789773 - %.02f%% similarity\n",clock_speed,clock_speed/1789773.0*100);
    //for test purpose: remove once done testing!!
    /*std::FILE* memory_dump = fopen("dump","w");
    fwrite(&cpu_ptr->memory[0x6004],sizeof(uint8_t),strlen((char*)(&cpu_ptr->memory[0x6004])),memory_dump);
    fclose(memory_dump);*/

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

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    char * vertex_source = new char[vertex_len+1];
    vertex_source[vertex_len] = 0;
    memcpy(vertex_source,vertex,vertex_len);

    glShaderSource(vertexShader,1,&vertex_source, NULL);
    glCompileShader(vertexShader);

    GLint compileStatus;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    char * fragment_source = new char[fragment_len+1];
    fragment_source[fragment_len] = 0;
    memcpy(fragment_source,fragment,fragment_len);

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
    while(!interrupted) {
        cpu_ptr->clock();
        total_ticks = cpu_ptr->cycles;
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

void sampleAPU() {
    int sr = apu_ptr->sample_rate;
    const int ns_wait = (1e9/(sr*1.024));
    long long loops = 0;
    int16_t buffer[BUFFER_LEN] = {0};
    int16_t buffer_copy[BUFFER_LEN] = {0};
    long long last_q = 0;
    while (!interrupted) {
        int16_t out = mix(apu_ptr);
        long long internal_nano = apu_ptr->cycles*2*1e9/cpu_ptr->CLOCK_SPEED;
        //out = 0;
        //std::lock_guard<std::mutex> lock(audioBufferMutex);
        //printf("queue: %i\n",out);
        while (loops<internal_nano/ns_wait) {
            //buffer[loops%BUFFER_LEN] = loops%BUFFER_LEN ? mix(apu_ptr) : 32767;
            buffer[loops%BUFFER_LEN] = mix(apu_ptr);
            //printf("%lf,%i\n",(double)loops/sr,buffer[loops%BUFFER_LEN]);
            loops++;
            if (loops%BUFFER_LEN==0) {
                int buffer_size = SDL_GetQueuedAudioSize(audio_device);
                
                memcpy(&buffer_copy,&buffer,BUFFER_LEN*sizeof(int16_t));
                if (buffer_size>BUFFER_LEN*sizeof(int16_t)*2) { //clocked to run a little bit faster, so we must account for a slight overflow in samples - better than sending too little
                    //printf("overflow\n");
                    //printf("%i\n",buffer_size);
                    SDL_DequeueAudio(audio_device,&buffer_copy,sizeof(int16_t)*BUFFER_LEN);
                } else {
                    SDL_QueueAudio(audio_device,&buffer_copy,sizeof(int16_t)*BUFFER_LEN);
                }
            }
            if (internal_nano/ns_wait>last_q+1) {
                //printf("skipped smth: %lli %lli\n",internal_nano/ns_wait,last_q);
            }
            last_q = internal_nano/ns_wait;
        }
        std::this_thread::sleep_for(std::chrono::nanoseconds(ns_wait));
        //audio_buffer[buffer_ind++] = out;
        //if (buffer_ind>=BUFFER_LEN) {
            //buffer_ind = 0;
        //}

    }
}

void NESLoop() {
    
    const double ns_wait = 1e9/cpu_ptr->CLOCK_SPEED;
    printf("Elapsed: %lf\n",ns_wait);
    void* system[3] = {cpu_ptr,ppu_ptr,apu_ptr};
    //emulator loop
    while (!interrupted) {
        if (!paused) {
            //if (clock_speed<=cpu_ptr->CLOCK_SPEED) { //limit clock speed
            //printf("clock speed: %i\n",cpu_ptr->emulated_clock_speed());
            cpu_ptr->clock();
            // 3 dots per cpu cycle
            total_ticks = cpu_ptr->cycles;
            clock_speed = cpu_ptr->emulated_clock_speed();

            while (apu_ptr->cycles*2<cpu_ptr->cycles) {
                apu_ptr->cycle();
                //apu_ptr->cycles++;
            }
            while (ppu_ptr->cycles<(cpu_ptr->cycles*3)) {
                ppu_ptr->cycle();
                cpu_ptr->rom->get_mapper()->clock(&system[0]);
                
                
                if (ppu_ptr->debug) {
                    printf("PPU REGISTERS: ");
                    printf("VBLANK: %i, PPUCTRL: %02x, PPUMASK: %02x, PPUSTATUS: %02x, OAMADDR: N/A (so far), PPUADDR: %04x\n",ppu_ptr->vblank, (uint8_t)cpu_ptr->memory[0x2000],(uint8_t)cpu_ptr->memory[0x2001],(uint8_t)cpu_ptr->memory[0x2002],ppu_ptr->v);
                    printf("scanline: %i, cycle: %i\n",ppu_ptr->scanline,ppu_ptr->scycle);
                }
                //printf("%i\n",ppu.v);
            }
            real_time = epoch_nano()-start_nano;
            long long cpu_time = ns_wait*cpu_ptr->cycles;
            int diff = cpu_time-(real_time-paused_time);
            if (diff > 0) {
                std::this_thread::sleep_for(std::chrono::nanoseconds(diff));
            }
        }
        
    }
    
}

void placeholder() {

}

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

int main(int argc, char ** argv) {
    std::signal(SIGINT,quit);
    std::signal(SIGSEGV,quit);
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
    ROM rom;
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

    char* filename = new char[strlen(ROM_NAME)+1];
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
    if (os != -1) {
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
        }
    } else {
        printf("OS not detected. No save folder created.\n");
    }

    // SDL initialize
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK);
    //SDL_Vulkan_LoadLibrary(nullptr);
    SDL_ShowCursor(0);
    int controller_index = 0;
    controller = SDL_JoystickOpen(controller_index);
    auto c_name = SDL_JoystickNameForIndex(controller_index);
    printf("%s\n",c_name ? c_name : "null");
    printf("SDL Initialized\n");

    //set display dimensions
    SDL_DisplayMode DM;
    SDL_GetDesktopDisplayMode(0,&DM);
    WINDOW_INIT[0] = DM.w*init_window_scale;
    WINDOW_INIT[1] = DM.h*init_window_scale;

    //audio
    audio_spec.freq = 44100;
    audio_spec.format = AUDIO_S16;  // 16-bit signed, little-endian
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
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    printf("GL Major version: 3\n");
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 2);
    printf("GL Minor version: 2\n");
    int* viewport = new int[4];
    viewportBox(&viewport,WINDOW_INIT[0],WINDOW_INIT[1]);
    printf("Viewport set\n");
    SDL_Window* window = SDL_CreateWindow(filename,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WINDOW_INIT[0],WINDOW_INIT[1],FLAGS);
    printf("Made window\n");

    SDL_SetWindowTitle(window,filename);
    printf("Window Title Set\n");
    SDL_GLContext context = SDL_GL_CreateContext(window);
    glewExperimental = GL_TRUE;
    glewInit();
    glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);
    printf("OpenGL Initialized.\n");
    GLenum error = glGetError();
    SDL_Event event;
    

    //Dear Imgui init
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGuiIO io = ImGui::GetIO();
    ImGui_ImplSDL2_InitForOpenGL(window,context);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    // Shader init
    init_shaders();
    printf("Shaders compiled and linked.\n");
    bool changed_use_shaders = false;
    //ntsc filter init

    /* pass it the buffer to be drawn on screen */
    unsigned char * filtered = (unsigned char*)malloc(NES_DIM[0]*NES_DIM[1]*filtered_res_scale*filtered_res_scale*3*sizeof(char));
    crt_init(&crt, NES_DIM[0]*filtered_res_scale,NES_DIM[1]*filtered_res_scale, CRT_PIX_FORMAT_RGB, &filtered[0]);
    /* specify some settings */
    crt.blend = 0;
    crt.scanlines = 1;


    //VBO & VAO init for the fullscreen texture
    GLuint VBO, VAO;
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
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, NES_DIM[0],NES_DIM[1], 0, GL_RGB, GL_UNSIGNED_BYTE, out_img);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenerateMipmap(GL_TEXTURE_2D);

    glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"), 0);
    
    printf("Window texture bound and mapped.\n");

    //Enter NES logic loop alongside window loop
    start = epoch();
    start_nano = epoch_nano();

    CPU cpu(false);
    cpu_ptr = &cpu;
    printf("CPU Initialized.\n");
    static APU apu;
    apu.sample_rate = audio_spec.freq;
    apu.device = audio_device;
    cpu.apu = &apu;
    apu_ptr = &apu;
    apu.cpu = &cpu;
    printf("APU set\n");

    cpu.loadRom(&rom);
    cpu.reset();
    printf("ROM loaded into CPU.\n");

    PPU ppu(&cpu);
    ppu_ptr = &ppu;
    ppu.debug = false;
    printf("PPU Initialized\n");
    std::thread NESThread(NESLoop);
    std::thread sampleGet(sampleAPU);
    //std::thread tCPU(CPUThread);
    //std::thread tPPU(PPUThread);
    //std::thread AudioThread(AudioLoop);

    printf("NES thread started. Starting main window loop...\n");
    printf("x,y\n");
    float t_time = SDL_GetTicks()/1000.0;
    float last_time = SDL_GetTicks()/1000.0;
    int16_t buffer[BUFFER_LEN*2];
    SDL_PauseAudioDevice(audio_device,0);
    //main window loop
    while (!interrupted) {
        //check if checkbox was clicked
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

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();
        if (!paused) {
            //printf("size 16 sprites: %i\n",*(ppu_ptr->PPUCTRL)&0x20);
            //ppu_ptr->image_mutex.lock();
            float diff = t_time-last_time;
            //char * new_title = new char[255];
            //sprintf(new_title,"%s - %.02f FPS",filename,1/diff);
            //SDL_SetWindowTitle(window,new_title);
            last_time = SDL_GetTicks()/1000.0;
        }
        //logic is executed in nes thread
        //apply ntsc filter before drawing
        if (use_shaders) {
            ntsc.data = out_img; /* buffer from your rendering */
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
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, NES_DIM[0],NES_DIM[1], 0, GL_RGB, GL_UNSIGNED_BYTE, out_img);
            
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
        if (paused) {
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
        //update screen
        //reset viewport
        int* new_viewport = new int[4];
        SDL_GetWindowSize(window,&new_viewport[2],&new_viewport[3]);
        viewportBox(&new_viewport,new_viewport[2],new_viewport[3]);
        glViewport(new_viewport[0],new_viewport[1],new_viewport[2],new_viewport[3]);
        delete[] new_viewport;

        SDL_GL_SwapWindow(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ppu_ptr->image_mutex.unlock();
        // event loop
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
                    if (changing_keybind>-1) {
                        mapped_keys[changing_keybind] = event.key.keysym.scancode;
                        changing_keybind = -1;
                    }
                    switch (event.key.keysym.sym) {
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
                        case SDLK_r:
                            {
                            SDL_Scancode modifier = SDL_SCANCODE_LCTRL;
                            #ifdef __APPLE__
                                modifier = SDL_SCANCODE_LGUI;
                            #endif
                            
                            if (state[modifier] && !paused) { //ctrl+r (or cmd+r) - reset shortcut
                                interrupted = true;
                                NESThread.join();
                                sampleGet.join();
                                cpu.init_vals();
                                rom.reset_mapper();
                                cpu.loadRom(&rom);
                                cpu.reset();
                                interrupted = false;
                                NESThread = std::thread(NESLoop);
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
                            cpu.last = epoch_nano(); // reset timing
                            break;
                    }
            }
            ImGui_ImplSDL2_ProcessEvent(&event);
        }
        SDL_Delay(1000/desired_fps);
        t_time = SDL_GetTicks()/1000.0;
    }
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
    delete[] original_start;
    delete[] audio_buffer;
    printf("Quit successfully.\n");
    //tCPU.join();
    //tPPU.join();
    //stbi_image_free(img);
    return 0;
}