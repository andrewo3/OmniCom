#ifndef NESSYS_H
#define NESSYS_H

#include "cpu.h"
#include "ppu.h"
#include "apu.h"
#include "window/window.h"
#include <thread>
#include <vector>


class BaseSystem {
    public:
        virtual void Loop() = 0;
        virtual void AudioLoop() = 0;
        virtual void Start() = 0;
        virtual void Cycle() = 0;
        virtual void Save() = 0;
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

};

namespace NES {

class CPU;
class PPU;
class APU;

class System: public BaseSystem {
    public:
        System();
        void Loop();
        void AudioLoop();
        void Start();
        void Cycle();
        void Save();
        void Stop();
        bool Render();
        void Update();
        void GLSetup();
        void SetController(NES::Controller* cont, int port);
        void loadRom(long len, uint8_t* data);
        CPU* cpu;
        PPU* ppu;
        APU* apu;
        ROM* rom;
        NES::Controller* cont1;
        NES::Controller* cont2;
        std::thread loop_thread;
        std::thread audio_thread;
        long long start_time;
        long long paused_time = 0;
        long long pause_start = 0;
        //std::vector<int16_t> audio_queue;
        const int video_dim[2] = {256,240};
    private:
        int clock_diff;
        bool fullscreen = false;

        GLuint texture;
        GLuint VBO, VAO; 
        GLuint shaderProgram;
        GLuint vertexShader;
        GLuint fragmentShader;
        void initShaders();   

};

}

#endif