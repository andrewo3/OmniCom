#ifndef SNES_SYS_H
#define SNES_SYS_H

#include "system.h"
#include "rom.h"
#include "cpu.h"
#include <thread>

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
        System();
        void Loop();
        void AudioLoop();
        void Cycle();
        void Save(FILE* save_file);
        void Load(FILE* load_file);
        bool Render();
        void Update();
        void Stop();
        void GLSetup();
        void Start();
        void loadRom(long len, uint8_t* data);
        ROM* rom;
        CPU* cpu;
        std::thread loop_thread;
        volatile bool stop = true; //for debugging - remove later
};

}

#endif