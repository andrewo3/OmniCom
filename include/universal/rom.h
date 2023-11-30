#ifndef rom_h
#define rom_h
#include <cstdint>
#include <cstdio>

enum NT_MIRROR { HORIZONTAL, VERTICAL, FOURSCREEN };

class ROM {
    public:
        ROM();
        ROM(const char* src);
        void load_file(const char* src);
        const char* src_filename;
        bool is_valid() { return valid_rom; }
        int8_t* prg;
        int8_t* chr;
        int8_t *get_prg_bank(int bank_num);
        int8_t *get_chr_bank(int bank_num);
        uint8_t get_mapper() {return mapper;}
        int get_prgsize() {return prgsize;}
        int get_chrsize() {return chrsize;}
        ~ROM();
        NT_MIRROR mirrormode;
        int8_t* rom_mirror(int8_t* address);
    private:
        bool valid_rom = false;
        bool nes2 = false;
        int filename_length;
        char header[16];
        int8_t trainer[512]; //load at $7000 if present
        int prgsize;
        int chrsize;
        uint8_t mapper;
};

#endif