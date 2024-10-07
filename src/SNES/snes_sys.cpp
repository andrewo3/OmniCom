#include "snes_sys.h"
#include <cstdio>
#include <cstdbool>
#include <string>
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
    std::string types[3] = {"Lo","Hi","ExHi"};
    char name[21];
    uint8_t header_size = 0x20;
    uint16_t checksum = 0;
    for (long b=0; b<len; b++) {
        checksum+=data[b];
    }
    int type = -1;
    printf("Checksum: %04x - Header possibilities:\n",checksum);
    for (int h=0; h<3; h++) {
        uint16_t head_check = *(uint16_t*)(data+header_locs[h]+30);
        printf("Header defined checksum: %04x\n",head_check);
        for (int i=0; i<header_size; i++) {
            printf("%02x ",data[header_locs[h]+i]);
        }
        printf("\n");
        if (head_check == checksum) {
            type = h;
            break;
        }
    }
    if (type!=-1) {
        memcpy(name,&data[header_locs[type]],21);
        printf("Type: %s, ROM Internal Name: %s\n",types[type].c_str(),name);
    } else {
        printf("No valid checksum found.\n");
    }

}