#include "snes_sys.h"
#include <cstdio>
#include <cstdbool>
#include "SDL2/SDL.h"

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
    //printf("Render frame\n");
    return true;
}
void System::Update() {
    //printf("Update: Process Events\n");
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
    printf("load rom, length: %li\n",len);
    data += len%1024; //only consider data following copier header (if it exists)
    printf("Header present: %i\n",len%1024==512);

    //determine type of ROM by verifying where the header is.
    uint32_t header_locs[3] = {0x007FC0,0x00FFC0,0x40FFC0}; //Lo, Hi, ExHi
    uint8_t header_size = 0x20;
    uint16_t checksum = 0;
    for (uint8_t b=0; b<len; b++) {
        checksum+=data[b];
    }
    printf("Checksum: %04x - Header possibilities:\n",checksum);
    for (int h=0; h<3; h++) {
        for (int i=0; i<header_size; i++) {
            printf("%02x ",data[header_locs[h]+i]);
        }
        printf("\n");
    }

}