#ifndef MAPPER_H
#define MAPPER_H

#include <cstdint>
#include <cstdio>

class CPU;

class Mapper {
    public:
        int type;
        virtual void map_write(void** ptrs,int8_t* address, int8_t *val) = 0;
        virtual void map_read(void** ptrs, int8_t* address) = 0;
        virtual void clock(void** system) = 0;
};

class DEFAULT_MAPPER: public Mapper {
    public:
        virtual void map_write(void** ptrs,int8_t* address, int8_t *val) {}
        virtual void map_read(void** ptrs, int8_t* address) {}
        virtual void clock(void** system) {}
        DEFAULT_MAPPER(int t) {
            type = t;
        }
};

class NROM: public Mapper {
    public:
        virtual void map_write(void** ptrs,int8_t* address, int8_t *val) {}
        virtual void map_read(void** ptrs, int8_t* address) {}
        virtual void clock(void** system) {}
        NROM() {
            type = 0;
        }
};

class MMC1: public Mapper {
    public:
        virtual void map_write(void** ptrs,int8_t* address, int8_t *val) {}
        virtual void map_read(void** ptrs, int8_t* address) {}
        virtual void clock(void** system) {}
        MMC1() {
            type = 1;
        }
};

class UxROM: public Mapper {
    public:
        virtual void map_write(void** ptrs,int8_t* address, int8_t *val);
        virtual void map_read(void** ptrs, int8_t* address) {}
        virtual void clock(void** system) {}
        UxROM() {
            type = 2;
        }
};

class CNROM: public Mapper {
    public:
        virtual void map_write(void** ptrs,int8_t* address, int8_t *val);
        virtual void map_read(void** ptrs, int8_t* address){}
        virtual void clock(void** system) {}
        CNROM() {
            type = 3;
        }
};

class MMC3: public Mapper {
    public:
        virtual void map_write(void** ptrs,int8_t* address, int8_t *val);
        virtual void map_read(void** ptrs, int8_t* address){}
        virtual void clock(void** system);
        void scanline_clock(CPU* cpu);
        MMC3() {
            type = 4;
        }
    private:
        uint8_t reg = 0;
        uint8_t xbase = 0;
        bool wp = false;
        bool prgram = true;
        bool irq_enabled = true;
        uint16_t last_v = 0;
        int irq_counter = 255;
        uint8_t irq_reload = 255;
        bool scanline_counted = false;
};

#endif