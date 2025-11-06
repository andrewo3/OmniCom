#include "nes_sys.h"
#ifndef WEB
#include "GL/glew.h"
#else
#include <GLES3/gl3.h>
#endif
#include "util.h"
#include "glob_const.h"
#include "shader_data.h"
#include <thread>
#include <vector>
#include <condition_variable>

auto now = std::chrono::system_clock::now;

using namespace NES;

System::System() {
    //initialize the NES system
    cpu = new CPU(false);
    ppu = new PPU(cpu);
    apu = new APU();
    cpu->apu = apu;
    apu->setCPU(cpu);
    cont1 = new NES::Controller();
    cont2 = new NES::Controller();
    SetController(cont1,0);
    SetController(cont2,1);
    clock_diff = (int)1e9/cpu->CLOCK_SPEED;

}

void System::AudioLoop() {
    apu->setSampleRate(window->getAudioSpec().freq);
    int sr = apu->sample_rate;
    long long loops = 0;
    int16_t* buffer = new int16_t[BUFFER_LEN];
    int16_t* buffer_copy = new int16_t[BUFFER_LEN];
    long long last_q = 0;
    long long last_cycles = 0;
    long long cycles_per_audio = cpu->CLOCK_SPEED/sr;
    while (running) {
        apu->queue_mutex.lock();
        if (apu->queue_audio_flag) {
            int buffer_size = SDL_GetQueuedAudioSize(window->audio_device); 
            //int buffer_size = audio_queue.size()*sizeof(int16_t);
            if (buffer_size>BUFFER_LEN*sizeof(int16_t)*8) { //we must account for a slight overflow in samples
                //audio_queue.erase(audio_queue.end()-BUFFER_LEN,audio_queue.end());
                SDL_DequeueAudio(window->audio_device,nullptr,sizeof(int16_t)*BUFFER_LEN);
            } else {
                for (int i=0; i<BUFFER_LEN; i++) {
                    apu->buffer_copy[i]*=global_db;
                }
                //audio_queue.insert(audio_queue.end(),apu->buffer_copy,apu->buffer_copy+BUFFER_LEN);
                SDL_QueueAudio(window->audio_device,apu->buffer_copy,sizeof(int16_t)*BUFFER_LEN);
            }
            apu->queue_audio_flag = false;
            apu->queue_mutex.unlock();
        }
    }
    delete[] buffer;
    delete[] buffer_copy;
}

void System::Cycle() {
    while (apu->cycles*2<cpu->cycles) {
        apu->cycle();
    }
    while (ppu->cycles<(cpu->cycles*3)) {
        ppu->cycle();
    }
    cpu->clock();
}

void System::Save(FILE* save_file) {
    if (save_file != nullptr) {
        cpu->save_state(save_file);
    } else {
        saveRAM();
    }
}

void System::Load(FILE* load_file) {
    cpu->load_state(load_file);
}

void System::saveRAM() {
    if (rom->battery_backed) {
        printf("save ram\n");
        std::FILE* ram_save = fopen((config_dir+sep+std::string("ram")).c_str(),"wb");
        cpu->save_ram(ram_save);
        fclose(ram_save);
    }
}

void System::loadRom(long len, uint8_t* data) {
    rom = new ROM(len,data);
    printf("Rom object created.\n");
    cpu->loadRom(rom);
    printf("Rom loaded into CPU.\n");
    ppu->loadRom(rom);
    printf("Rom loaded into PPU.\n");
    cpu->reset();
}

void System::Start() {
    cpu->reset();
    apu->enabled[0] = false;
    apu->enabled[1] = false;
    apu->enabled[2] = false;
    apu->enabled[3] = false;
    apu->enabled[4] = false;
    running = true;
    start_time = epoch_nano();
    paused_time = 0;
    loop_thread = std::thread(&System::Loop, this);
    audio_thread = std::thread(&System::AudioLoop,this);
}

void System::Stop() {
    paused = false;
    pause_cv.notify_all();
    paused_window = false;
    pause_mut.unlock();
    pause_cv.notify_all();
    running = false;
    int clock_speed = cpu->emulated_clock_speed(epoch_nano()-start_time-paused_time);
    printf("Cycles: %lli, Emulated Clock Speed: %i - Target: (approx.) 1789773 - %.02f%% similarity\n",cpu->cycles,clock_speed,clock_speed/1789773.0*100);
    saveRAM();
    audio_thread.join();
    loop_thread.join();
    
}

System::~System() {
    glDetachShader(shaderProgram,vertexShader);
    glDetachShader(shaderProgram,fragmentShader);
    glDeleteProgram(shaderProgram);
    delete cpu;
    delete ppu;
    delete apu;
    delete rom;
}

void System::Loop() {
    using namespace std::chrono;
    //emulator loop
    time_point<system_clock, nanoseconds> epoch;
    long long frame_count = 0;
    
    while (running) {
        if (!paused) {
            Cycle();

            //calculate delay
            if (ppu->frames>frame_count) { //only sleep on frame change
                frame_count = ppu->frames;
                draw_cv.notify_all();
                time_point<system_clock, nanoseconds> result_time = epoch+nanoseconds(start_time+paused_time)+nanoseconds((cpu->cycles*(int)1e9)/cpu->CLOCK_SPEED);
                if (result_time<now()) { 
                    //if its far behind (consider system sleep), fake a pause for the whole duration
                    paused_time = epoch_nano()-start_time-(cpu->cycles*(int)1e9)/cpu->CLOCK_SPEED;
                }
                //printf("wait time: %lli\n",result_time);
                std::this_thread::sleep_until(result_time);
            }
        } else {
            pause_cv.wait(pause_mut);
            pause_mut.unlock();
        }
        
    }
    apu->queue_mutex.unlock();
    
}

bool System::Render() {
    //reset viewport
    int w;
    int h;
    SDL_GetWindowSize(window->GetSDLWin(),&w,&h);
    setGLViewport(w,h,(float)video_dim[0]/video_dim[1]);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    ppu->image_mut.lock();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, video_dim[0],video_dim[1], 0, GL_RGB, GL_UNSIGNED_BYTE, ppu->getImg());
    ppu->image_mut.unlock();
    glUseProgram(shaderProgram);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glBindVertexArray(0);
    glUseProgram(0);
    ppu->image_drawn = true;
    return 1;
}

void System::Update() {
    SDL_PumpEvents();
    //update controllers
    bool controller1_inputs[8];
    bool controller2_inputs[8];
    for (int i=0; i<8; i++) {
        if (controller==NULL) {
            controller1_inputs[i] = state[mapped_keys[i]];
        } else {
            if (mapped_joy[i]&0x80) {
                if (joystickDir(controller)!=0) {
                    controller1_inputs[i] = joystickDir(controller)&mapped_joy[i]&0x7F;
                } else {
                    controller1_inputs[i] = SDL_JoystickGetHat(controller,0)&mapped_joy[i]&0x7F;
                }
            } else {
                controller1_inputs[i] = SDL_JoystickGetButton(controller,mapped_joy[i]);
            }
        }
        controller2_inputs[i] = false;
    }
    cont1->update_inputs(controller1_inputs);
    cont2->update_inputs(controller2_inputs);
    //process other events
    SDL_Event event;
    while(SDL_PollEvent(&event) && running) {
        switch(event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    int* new_viewport = new int[4];
                    int new_width = event.window.data1;
                    int new_height = event.window.data2;
                    setGLViewport(new_width,new_height,(float)video_dim[0]/video_dim[1]);
                } else if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                    running = false;
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
                            printf("Current Emulated Clock Speed: %i\n",cpu->emulated_clock_speed(epoch_nano()));
                        }
                        break;
                        }
                    #ifndef __EMSCRIPTEN__
                    case SDLK_F11:
                        {
                        fullscreen = fullscreen ? false : true;
                        SDL_Window* sdlwin = window->GetSDLWin();
                        SDL_SetWindowFullscreen(sdlwin,fullscreen*SDL_WINDOW_FULLSCREEN_DESKTOP);
                        SDL_SetWindowPosition(sdlwin,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED);
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
                            //reset here
                            Stop();
                            printf("Stopped.\n");
                            Start();
                            printf("Restarted.\n");
                        }
                        break;
                        }
                    case SDLK_ESCAPE:
                        if (paused) {
                            paused_time += epoch_nano()-pause_start;
                        } else {
                            pause_start = epoch_nano();
                        }
                        paused = paused ? false : true;
                        if (paused) {
                            pause_mut.lock();
                        } else {
                            pause_mut.unlock();
                            pause_cv.notify_all();
                        }
                        paused_window = true;
                        cpu->last = epoch_nano(); // reset timing
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
                    if (joystickDir(controller) != 0) {
                        mapped_joy[changing_keybind] = joystickDir(controller)|0x80;
                        changing_keybind = -1;
                    }
                }
                break;
        }
        if (paused) {
            ImGui_ImplSDL2_ProcessEvent(&event);
        }
    }
}

void System::initShaders() {
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

void System::GLSetup() {
    initShaders();
    printf("Shaders compiled and linked.\n");
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
    glBindTexture(GL_TEXTURE_2D,0);
    printf("Window texture bound and mapped.\n");
}

void System::SetController(NES::Controller* cont, int port) {
    cpu->set_controller(cont, port);
}