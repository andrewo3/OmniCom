#ifndef SYSTEM_H
#define SYSTEM_H
#include <cstdint>
#include <cstdio>

class EmuWindow;

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

};

#endif