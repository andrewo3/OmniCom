#include "rom.h"
#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>


ROM::ROM() {
    
}

ROM::ROM(const char* src) {
    this->load_file(src);

}

void ROM::load_file(const char* src) {
    this->src_filename = src;
    filename_length = strlen(src);
    FILE* rp = std::fopen(src_filename,"rb"); // rom pointer
    std::fread(header,16,1,rp);
    if (header[0]=='N' && header[1]=='E' && header[2]=='S' && header[3]==0x1a) {
        valid_rom = true;
    } else {
        return;
    }
    if (valid_rom && (header[7]&0x0C)==0x08) {
        nes2 = true;
    }

    bool trainer_present = header[6]&0x04;
    mapper = ((header[6]&0xF0)>>4)|(header[7]&0xF0);
    if (nes2) {
        int8_t msb = header[9]&0x0F;
        if (msb == 0x0F) { //use exponent notation
            prgsize = pow(2,(header[4]&0xFC)>>2)*((header[4]&0x3)*2+1);
        } else {
            prgsize = (header[4]|(msb)<<8)*0x4000;
        }
        msb = (header[9]&0xF0);
        if (msb == 0xF0) {
            chrsize = pow(2,(header[5]&0xFC)>>2)*((header[5]&0x3)*2+1);
        } else {
            chrsize = (header[5]|(header[9]&0xF0)<<4)*0x2000;
        }
        
    } else {
        prgsize = header[4]*0x4000;
        chrsize = header[5]*0x2000;
    }
    prg = (int8_t *)malloc(prgsize*sizeof(int8_t));
    chr = (int8_t *)malloc(chrsize*sizeof(int8_t));
    if (trainer_present) { // if trainer is present
        std::fread(trainer,512,1,rp);
    }

    std::fread(prg,prgsize,1,rp);
    std::fread(chr,chrsize,1,rp);

    std::fclose(rp);
}

int8_t* ROM::get_prg_bank(int bank_num) { //size = 0x4000
    if (bank_num>prgsize/0x4000) {
        printf("Bank number out of bounds\n");
        throw(1);
   }
    return prg+0x4000*bank_num;
}

ROM::~ROM() {
    free(prg);
    free(chr);
}