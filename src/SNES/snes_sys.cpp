#include "snes_sys.h"
#include "rom.h"
#include <cstdio>
#include <cstdbool>
#include <string>
#include <thread>
#include "SDL2/SDL.h"

using namespace SNES;

System::System() {
    cpu = new CPU();
    rom = new ROM();
    cpu->rom = rom;
    printf("Constructor\n");
}
void System::Loop() {
    printf("Loop\n");
    while (running) {
        cpu->clock();
    }
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
    delete cpu;
    delete rom->mem;
    delete rom;
    printf("Stop Emulation\n");
    running = false;
}
void System::GLSetup() {
    printf("OpenGL Setup\n");
}
void System::Start() {
    printf("start\n");
    cpu->rom = rom;
    cpu->reset();
    loop_thread = std::thread(&System::Loop, this);
    running = true;
}
void System::loadRom(long len, uint8_t* data) {
    printf("load rom, length: %li\n",len);
    int mod = len%1024;
    data += mod; //only consider data following copier header (if it exists)
    len -= mod;
    
    printf("Header present: %i\n",mod==512);
    rom->mem = new uint8_t[len];
    memcpy(rom->mem,data,len);
    //determine type of ROM by verifying where the header is.
    uint32_t header_locs[3] = {0x007FC0,0x00FFC0,0x40FFC0}; //Lo, Hi, ExHi
    std::string types[3] = {"Lo","Hi","ExHi"};
    uint8_t header_size = 0x20;
    uint16_t checksum = 0;
    for (long b=0; b<len; b++) {
        checksum+=data[b+mod];
    }
    int type = -1;
    int rom_speed = 0;
    bool ascii[3] = {true,true,true};
    printf("Checksum: %04x - Header possibilities:\n",checksum);
    for (int h=0; h<3; h++) {
        uint16_t head_check = *(uint16_t*)(data+header_locs[h]+30);
        printf("Header defined checksum: %04x\n",head_check);
        for (int i=0; i<header_size; i++) {
            printf("%02x ",data[header_locs[h]+i]);
        }
        printf("\n");
        if (head_check == checksum) {
            type = data[header_locs[h]+21]&0xf;
            if (type==5) {
                type == 2;
            }
            rom_speed = data[header_locs[h]+21]&0x10;
            break;
        }
        for (int i=0; i<21; i++) {
            char c = data[header_locs[h]+i];
            if (c&0x80) { //check if rom name only has ascii characters
                printf("Type: %s, no ascii.\n",types[h].c_str());
                ascii[h] = false;
                break;
            }
        }
    }
    if (type == -1) {
        printf("No valid checksum found - checking ascii characters...\n");
        uint8_t asc_ind = 0;
        for (int i=0; i<3; i++) {
            if (ascii[i]==true) { //find first rom with only ascii characters in internal name
                asc_ind = i;
                break;
            }
        }
        //use first rom if none have ascii characters
        type = data[header_locs[asc_ind]+21]&0xf;
        rom_speed = data[header_locs[asc_ind]+21]&0x10;
    }
    memcpy(rom->name,&data[header_locs[type]],21);
    rom->name[21] = 0;
    rom->type = type;
    rom->chipset = data[header_locs[type]+22];
    rom->rom_size_kb = 1<<data[header_locs[type]+23];
    rom->ram_size_kb = 1<<data[header_locs[type]+24];
    rom->country = data[header_locs[type]+25];
    printf("Type: %s, Speed: %s, ROM Internal Name: %s\n",types[type].c_str(),rom_speed?"Fast":"Slow",rom->name);

}