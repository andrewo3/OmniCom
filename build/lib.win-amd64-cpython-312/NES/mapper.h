#ifndef MAPPER_H
#define MAPPER_H

#include <cstdint>
#include <cstdio>
#include <cstring>


namespace NES {

class CPU;

class PPU;

class Mapper {
    public:
        int type;
        virtual void map_write(void** ptrs,int8_t* address, int8_t *val) = 0;
        virtual void map_read(void** ptrs, int8_t* address) = 0;
        virtual void clock(void** system) = 0;
        virtual void serialize(void** system, char* out) = 0;
        virtual void deserialize(void** system, char* in) = 0;
        virtual ~Mapper() = default;
};

class DEFAULT_MAPPER: public Mapper {
    public:
        virtual void map_write(void** ptrs,int8_t* address, int8_t *val) {}
        virtual void map_read(void** ptrs, int8_t* address) {}
        virtual void clock(void** system) {}
        DEFAULT_MAPPER(int t) {
            type = t;
        }
        virtual void serialize(void** system, char* out) {}
        virtual void deserialize(void** system, char* in) {}
};

class NROM: public Mapper {
    public:
        virtual void map_write(void** ptrs,int8_t* address, int8_t *val) {}
        virtual void map_read(void** ptrs, int8_t* address) {}
        virtual void clock(void** system) {}
        NROM() {
            type = 0;
        }
        virtual void serialize(void** system, char* out) {}
        virtual void deserialize(void** system, char* in) {}
};

class MMC1: public Mapper {
    public:
        virtual void map_write(void** ptrs,int8_t* address, int8_t *val);
        virtual void map_read(void** ptrs, int8_t* address) {}
        virtual void clock(void** system) {}
        uint8_t prg_mode = 3;
        uint8_t chr_mode = 0;
        MMC1() {
            type = 1;
        }
        uint8_t shift_reg = 0x10;
        uint8_t bank_reg = 0;
        virtual void serialize(void** system, char* out) {}
        virtual void deserialize(void** system, char* in) {}
        void control(CPU* cpu, PPU* ppu, uint8_t val);
};

class UxROM: public Mapper {
    public:
        virtual void map_write(void** ptrs,int8_t* address, int8_t *val);
        virtual void map_read(void** ptrs, int8_t* address) {}
        virtual void clock(void** system) {}
        UxROM() {
            type = 2;
        }
        virtual void serialize(void** system, char* out) {
            memcpy(out,&bank_num,sizeof(int));
        }
        virtual void deserialize(void** system, char* in);
    private:
        int bank_num = 0;
};

class CNROM: public Mapper {
    public:
        virtual void map_write(void** ptrs,int8_t* address, int8_t *val);
        virtual void map_read(void** ptrs, int8_t* address){}
        virtual void clock(void** system) {}
        CNROM() {
            type = 3;
        }
        virtual void serialize(void** system, char* out) {
            memcpy(out,&bank_num,sizeof(int));
        }
        virtual void deserialize(void** system, char* in);
    private:
        int bank_num = 0;
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
        virtual void serialize(void** system, char* out);
        virtual void deserialize(void** system, char* in);
    private:
        uint8_t reg = 0;
        uint8_t xbase = 0;
        bool wp = false;
        bool prgram = true;
        bool irq_enabled = true;
        uint16_t last_v = 0;
        int irq_counter = 255;
        uint8_t irq_reload = 255;
        uint8_t off_clocks = 0;
        bool scanline_counted = false;
};

class NTDEC2722: public Mapper {
    public:
        virtual void map_write(void** ptrs,int8_t* address, int8_t *val);
        virtual void map_read(void** ptrs, int8_t* address){}
        virtual void clock(void** system);
        NTDEC2722() {
            type = 40;
        }
        virtual void serialize(void** system, char* out) {}
        virtual void deserialize(void** system, char* in) {}
    private:
        uint8_t select = 0;
        uint16_t counter = 4096*3;
        bool enabled = true;
};

}

#endif