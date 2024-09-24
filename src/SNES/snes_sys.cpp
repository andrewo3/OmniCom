#include "snes_sys.h"

using namespace SNES;

System::System() {
    printf("Constructor\n");
}
void System::Loop() {
    printf("Loop\n");
}
void System::AudioLoop() {
    printf("Audio Loop\n");
}
void System::Cycle() {
    printf("Cycle\n");
}
void System::Save(FILE* save_file) {
    printf("Save State\n");
}
void System::Load(FILE* load_file) {
    printf("Load State\n");
}
bool System::Render() {
    printf("Render frame\n");
    return true;
}
void System::Update() {
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
void System::Stop() {
    printf("Stop Emulation\n");
}
void System::GLSetup() {
    printf("OpenGL Setup\n");
}
void System::Start() {
    printf("start\n");
    running = true;
}
void System::loadRom(long len, uint8_t* data) {
    printf("load rom\n");
}