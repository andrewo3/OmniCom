class Mapper {
    public:
        int type;
        void map_write(uint8_t ** val);
        void map_read(uint8_t** val);
};

class MMC1: public Mapper {
    public:
        int type = 1;
};