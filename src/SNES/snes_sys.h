#ifndef SNES_SYS_H
#define SNES_SYS_H

#include "system.h"

/* for reference
==============================
class BaseSystem {
    public:
        virtual void Loop() = 0;
        virtual void AudioLoop() = 0;
        virtual void Start() = 0;
        virtual void Cycle() = 0;
        virtual void Save(FILE* save_file) = 0;
        virtual void Load(FILE* load_file) = 0;
        virtual void Stop() = 0;
        virtual bool Render() = 0;
        virtual void Update() = 0;
        virtual void GLSetup() = 0;
        virtual void loadRom(long len, uint8_t* data) = 0;
        int video_dim[2];
        bool running = false;
        EmuWindow* window;
        void setWindow(EmuWindow* win) {
            window = win;
        }

};*/

namespace SNES {
class CPU;

class System: public BaseSystem {
    public:
        System() {
            printf("Constructor\n");
        }
        void Loop() {
            printf("Loop\n");
        }
        void AudioLoop() {
            printf("Audio Loop\n");
        }
        void Cycle() {
            printf("Cycle\n");
        }
        void Save(FILE* save_file) {
            printf("Save State\n");
        }
        void Load(FILE* load_file) {
            printf("Load State\n");
        }
        bool Render() {
            printf("Render frame\n");
            return true;
        }
        void Update() {
            printf("Update: Process Events\n");
            SDL_Event event;
            while(SDL_PollEvent(&event) && running) {
                switch(event.type) {
                    case SDL_QUIT:
                        running = false;
                        break;
                }
            }
        }
        void Stop() {
            printf("Stop Emulation\n");
        }
        void GLSetup() {
            printf("OpenGL Setup\n");
        }
        void Start() {
            printf("start\n");
            running = true;
        }
        void loadRom(long len, uint8_t* data) {
            printf("load rom\n");
        }
};

}

#endif