#include "rom.h"
#include "mapper.h"
#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>


ROM::ROM() { //if no rom specified, make dummy rom
    unsigned char dummy_prg[0x4010] = {0};
    dummy_prg[0] = 'N';
    dummy_prg[1] = 'E';
    dummy_prg[2] = 'S';
    dummy_prg[3] = 0x1a;
    dummy_prg[4] = 1;
    this->load_arr(0x4010,dummy_prg);
}

ROM::ROM(const char* src) {
    this->load_file(src);

}
ROM::ROM(int length, unsigned char* data) {
    this->load_arr(length,data);

}

void ROM::load_arr(int length, unsigned char* data) {
    int ind = 0;
    for (int i=0; i<16; i++) {
        header[i]=data[ind];
        ind++;
    }
    if (header[0]=='N' && header[1]=='E' && header[2]=='S' && header[3]==0x1a) {
        valid_rom = true;
    } else {
        return;
    }
    if (valid_rom && (header[7]&0x0C)==0x08) {
        nes2 = true;
    }

    bool trainer_present = header[6]&0x04;
    int mapper_num = ((header[6]&0xF0)>>4)|(header[7]&0xF0);
    switch (mapper_num) {
        case 0:
            mapper = new NROM();
            break;
        case 1:
            mapper = new MMC1();
            break;
        case 2:
            mapper = new UxROM();
            break;
        case 3:
            mapper = new CNROM();
            break;
        case 4:
            mapper = new MMC3();
            break;
        case 40:
            mapper = new NTDEC2722();
            break;
        default:
            mapper = new DEFAULT_MAPPER(mapper_num);
            printf("UNRECOGNIZED MAPPER!\n");
            break;
    }
    if (header[6]&0x08) {
        mirrormode = FOURSCREEN;
    } else {
        mirrormode = (header[6]&0x1) ? VERTICAL : HORIZONTAL;
    }
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
        printf("iNES\n");
        printf("%i\n",header[5]);
        prgsize = header[4]*0x4000;
        chrsize = header[5]*0x2000;
    }
    prg = (uint8_t *)malloc(prgsize*sizeof(uint8_t));
    chr = (uint8_t *)malloc(chrsize*sizeof(uint8_t));
    if (trainer_present) { // if trainer is present
        for (int i=0; i<512; i++) {
            trainer[i]=data[ind];
            ind++;
        }
    }

    for (int i=0; i<prgsize; i++) {
        prg[i]=data[ind];
        ind++;
    }
    for (int i=0; i<chrsize; i++) {
        chr[i]=data[ind];
        ind++;
    }
}

void ROM::reset_mapper() {
    int mapper_num = mapper->type;
    delete mapper;
    switch (mapper_num) {
        case 0:
            mapper = new NROM();
            break;
        case 1:
            mapper = new MMC1();
            break;
        case 2:
            mapper = new UxROM();
            break;
        case 3:
            mapper = new CNROM();
            break;
        case 4:
            mapper = new MMC3();
            break;
        case 40:
            mapper = new NTDEC2722();
            break;
        default:
            mapper = new DEFAULT_MAPPER(mapper_num);
            printf("UNRECOGNIZED MAPPER!\n");
            break;
    }
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
    int mapper_num = ((header[6]&0xF0)>>4)|(header[7]&0xF0);
    switch (mapper_num) {
        case 0:
            mapper = new NROM();
            break;
        case 1:
            mapper = new MMC1();
            break;
        case 2:
            mapper = new UxROM();
            break;
        case 3:
            mapper = new CNROM();
            break;
        case 4:
            mapper = new MMC3();
            break;
        case 40:
            mapper = new NTDEC2722();
            break;
        default:
            mapper = new DEFAULT_MAPPER(mapper_num);
            printf("UNRECOGNIZED MAPPER!\n");
            break;
    }
    if (header[6]&0x08) {
        mirrormode = FOURSCREEN;
    } else {
        mirrormode = (header[6]&0x1) ? VERTICAL : HORIZONTAL;
    }
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
        printf("iNES\n");
        printf("%i\n",header[5]);
        prgsize = header[4]*0x4000;
        chrsize = header[5]*0x2000;
    }
    prg = (uint8_t *)malloc(prgsize*sizeof(uint8_t));
    chr = (uint8_t *)malloc(chrsize*sizeof(uint8_t));
    if (trainer_present) { // if trainer is present
        std::fread(trainer,512,1,rp);
    }

    std::fread(prg,prgsize,1,rp);
    std::fread(chr,chrsize,1,rp);

    std::fclose(rp);
}

uint8_t* ROM::get_prg_bank(int bank_num) { //gets banks in 1 KB units
    return prg+0x400*(bank_num%(prgsize/0x400));
}

uint8_t* ROM::get_chr_bank(int bank_num) { //gets banks in 1 KB units
    if (chrsize==0) { // using chr-ram
        //something else will be done
        //printf("CHR-RAM\n");
        return chr_ram;
    }
    return chr+0x400*(bank_num%(chrsize/0x400));
}

ROM::~ROM() {
    free(prg);
    free(chr);
    delete mapper;
}