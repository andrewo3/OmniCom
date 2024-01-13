#ifndef MAPPER_H
#define MAPPER_H

#include <cstdint>
#include <cstdio>

class Mapper {
    public:
        int type;
        virtual void map_write(void** ptrs,int8_t* address, int8_t val) = 0;
        virtual void map_read(void** ptrs, int8_t* address) = 0;
};

class DEFAULT_MAPPER: public Mapper {
    public:
        virtual void map_write(void** ptrs,int8_t* address, int8_t val) {}
        virtual void map_read(void** ptrs, int8_t* address) {}
        DEFAULT_MAPPER(int t) {
            type = t;
        }
};

class NROM: public Mapper {
    public:
        virtual void map_write(void** ptrs,int8_t* address, int8_t val) {}
        virtual void map_read(void** ptrs, int8_t* address) {}
        NROM() {
            type = 0;
        }
};

class MMC1: public Mapper {
    public:
        virtual void map_write(void** ptrs,int8_t* address, int8_t val) {}
        virtual void map_read(void** ptrs, int8_t* address) {}
        MMC1() {
            type = 1;
        }
};

class UxROM: public Mapper {
    public:
        virtual void map_write(void** ptrs,int8_t* address, int8_t val) {}
        virtual void map_read(void** ptrs, int8_t* address) {}
        UxROM() {
            type = 2;
        }
};

class CNROM: public Mapper {
    public:
        virtual void map_write(void** ptrs,int8_t* address, int8_t val);
        virtual void map_read(void** ptrs, int8_t* address){}
        CNROM() {
            type = 3;
        }
};

class MMC3: public Mapper {
    public:
        virtual void map_write(void** ptrs,int8_t* address, int8_t val);
        virtual void map_read(void** ptrs, int8_t* address){}
        MMC3() {
            type = 4;
        }
    private:
        uint8_t reg = 0;
};

#endif