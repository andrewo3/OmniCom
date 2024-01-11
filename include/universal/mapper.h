#ifndef MAPPER_H
#define MAPPER_H

#include <cstdint>

class Mapper {
    public:
        int type;
        void map_write(uint8_t ** val);
        void map_read(uint8_t** val);
};

class NROM: public Mapper {
    public:
        int type = 0;
};

class MMC1: public Mapper {
    public:
        int type = 1;
};

#endif