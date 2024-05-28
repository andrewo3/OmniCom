#ifndef rom_h
#define rom_h
#include "mapper.h"
#include <cstdint>
#include <cstdio>

enum NT_MIRROR { HORIZONTAL, VERTICAL, FOURSCREEN, SINGLESCREEN };
 
class ROM {
    public:
        ROM();
        ROM(const char* src);
        ROM(int length, unsigned char* data);
        void load_arr(int length, unsigned char* data);
        void load_file(const char* src);
        const char* src_filename;
        bool is_valid() { return valid_rom; }
        bool battery_backed = false;
        uint8_t* prg;
        uint8_t* chr;
        uint8_t mmc1shift = 0x10;
        uint8_t mmc1bankmode = 0x03;
        uint8_t mmc1chrbank = 0x00;
        uint8_t mmc1prgbank = 0x00;
        uint8_t mmc1chrloc0 = 0;
        uint8_t mmc1chrloc1 = 0;
        uint8_t mmc1prgloc = 0;
        uint8_t *get_prg_bank(int bank_num);
        uint8_t *get_chr_bank(int bank_num);
        Mapper* get_mapper() {return mapper;}
        void reset_mapper();
        int get_prgsize() {return prgsize;}
        int get_chrsize() {return chrsize;}
        uint8_t *chr_ram;
        ~ROM();
        NT_MIRROR mirrormode;
    private:
        bool valid_rom = false;
        bool nes2 = false;
        int filename_length;
        char header[16];
        int8_t trainer[512]; //load at $7000 if present
        int prgsize;
        int chrsize;
        Mapper* mapper;
};

#endif