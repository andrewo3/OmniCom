class ROM {
    public:
        ROM(const char* src);
        const char* src_filename;
        bool is_valid() { return valid_rom; }
        int8_t* prg;
        int8_t* chr;
        int8_t get_prg_bank(int bank_num)[16384];
    private:
        bool valid_rom = false;
        bool nes2 = false;
        int filename_length;
        char header[16];
        int8_t trainer[512]; //load at $7000 if present
};