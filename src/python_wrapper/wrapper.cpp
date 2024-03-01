#include "util.h"
#include "rom.h"
#include "cpu.h"
#include "ppu.h"
#include "mapper.h"
#include "apu.h"
#include "pybind11/pybind11.h"
#include "pybind11/numpy.h"
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
        py::array_t<uint8_t> cpuMem();

    private:
        CPU* cpu;
        PPU* ppu;
        APU* apu;
        ROM* rom;
};

NES::NES(char* rom_name) {
    rom = new ROM(rom_name);
    cpu = new CPU(false);
    //ppu = new PPU(cpu);
    //apu = new APU();
    //apu->cpu = cpu;
    //cpu->apu = apu;
}

py::array_t<uint8_t> NES::cpuMem() {
    uint8_t* tmp = (uint8_t*)cpu->memory;
    py::capsule cleanup(tmp,[](void *f){});
    return py::array_t<uint8_t>(
        {0x10000},
        {sizeof(uint8_t)},
        tmp,
        cleanup
    );
}

NES::~NES() {
    delete rom;
    delete cpu;
    delete ppu;
    delete apu;
}

PYBIND11_MODULE(pyNES,m) {
    py::class_<NES>(m,"NES").def(py::init<char*>())
    .def("cpuMem",&NES::cpuMem);
}
