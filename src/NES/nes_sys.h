#ifndef NESSYS_H
#define NESSYS_H

#include "cpu.h"
#include "ppu.h"
#include "apu.h"
#include "window/window.h"
#include "system.h"
#include <thread>
#include <vector>

namespace NES {

class CPU;
class PPU;
class APU;

class System: public BaseSystem {
    public:
        System();
        ~System();
        void Loop();
        void AudioLoop();
        void Start();
        void Cycle();
        void Save(FILE* save_file);
        void Load(FILE* load_file);
        void saveRAM();
        void Stop();
        bool Render();
        void Update();
        void GLSetup();
        void SetController(NES::Controller* cont, int port);
        void loadRom(long len, uint8_t* data);
        CPU* cpu = nullptr;
        PPU* ppu = nullptr;
        APU* apu = nullptr;
        ROM* rom = nullptr;
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