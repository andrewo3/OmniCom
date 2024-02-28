#include "util.h"
#include "rom.h"
#include "cpu.h"
#include "ppu.h"
#include "mapper.h"
#include "apu.h"
#include "pybind11/pybind11.h"
#include <cstdint>


namespace py = pybind11;

class CPU;
class PPU;
class APU;
class ROM;

class NES {
    public:
        NES(char* rom_name);
        ~NES();
        int mapper;
        uint8_t* cpu_mem;
        uint8_t* ppu_mem;
    private:
        CPU* cpu;
        PPU* ppu;
        APU* apu;
        ROM* rom;
};

NES::NES(char* rom_name) {
    rom = new ROM(rom_name);
    cpu = new CPU();
    ppu = new PPU(cpu);
    apu = new APU();
    apu->cpu = cpu;
    cpu->apu = apu;
}

NES::~NES() {
    delete rom;
    delete cpu;
    delete ppu;
    delete apu;
}

PYBIND11_MODULE(pyNES,m) {
    py::class_<NES>(m,"NES").def(py::init<char*>());
}
