#include "util.h"
#include "rom.h"
#include "cpu.h"
#include "ppu.h"
#include "mapper.h"
#include "apu.h"
#include "pybind11/pybind11.h"
#include "pybind11/numpy.h"
#include <cstdint>
#include <thread>
#include <chrono>
#include <vector>

enum class Button {
    A = 0,
    B = 1,
    SELECT = 2,
    START = 3,
    UP = 4,
    DOWN = 5,
    LEFT = 6,
    RIGHT = 7
};


namespace py = pybind11;


class CPU;
class PPU;
class APU;
class ROM;

class ControllerWrapper {
    public:
        Controller cont;
        ControllerWrapper() {cont = Controller();}
        void updateInputs(py::list inputs);
};

void ControllerWrapper::updateInputs(py::list inputs) {
    bool data[8];
    for (int i=0; i<8; i++) {
        data[i] = py::cast<bool>(inputs[i]);
    }
    cont.update_inputs(data);
}

class NES {
    public:
        NES(char* rom_name);
        ~NES();
        int mapper;
        py::array_t<uint8_t> cpuMem();
        py::array_t<uint8_t> getImg();
        void setController(ControllerWrapper& cont,int port);
        void start();
        void stop();
        void operation_thread();
        void save(char * name);
        void load(char * name);
        
        Controller cont1;
        Controller cont2;

    private:
        CPU* cpu;
        PPU* ppu;
        APU* apu;
        ROM* rom;
        bool running = false;
        bool paused = false;
        long long paused_time = epoch_nano();
        std::thread running_t;
};

NES::NES(char* rom_name) {
    rom = new ROM(rom_name);
    cpu = new CPU(false);
    apu = new APU();
    apu->setCPU(cpu);
    cpu->apu = apu;
    cpu->loadRom(rom);
    cont1 = Controller();
    cont2 = Controller();
    cpu->set_controller(&cont1,0);
    cpu->set_controller(&cont2,1);
    cpu->reset();
    ppu = new PPU(cpu);


}

void NES::save(char* name) {
    
}

void NES::load(char* name) {

}

void NES::setController(ControllerWrapper& cont, int port) {
    cpu->set_controller(&(cont.cont),port);
}

void NES::operation_thread() {
    
    const double ns_wait = 1e9/cpu->CLOCK_SPEED;
    printf("Elapsed: %lf\n",ns_wait);
    long long real_time = 0;
    long long cpu_time;
    long long start_nano = epoch_nano();
    void* system[3] = {cpu,ppu,apu};
    //emulator loop
    while (running) {
        if (!paused) {
            //if (clock_speed<=cpu_ptr->CLOCK_SPEED) { //limit clock speed
            //printf("clock speed: %i\n",cpu_ptr->emulated_clock_speed());
            cpu->clock();

            while (apu->cycles*2<cpu->cycles) {
                apu->cycle();
                //apu_ptr->cycles++;
            }

            // 3 dots per cpu cycle
            while (ppu->cycles<(cpu->cycles*3)) {
                ppu->cycle();
                cpu->rom->get_mapper()->clock(&system[0]);
                
                if (ppu->debug) {
                    printf("PPU REGISTERS: ");
                    printf("VBLANK: %i, PPUCTRL: %02x, PPUMASK: %02x, PPUSTATUS: %02x, OAMADDR: N/A (so far), PPUADDR: %04x\n",ppu->vblank, (uint8_t)cpu->memory[0x2000],(uint8_t)cpu->memory[0x2001],(uint8_t)cpu->memory[0x2002],ppu->v);
                    printf("scanline: %i, cycle: %i\n",ppu->scanline,ppu->scycle);
                }
                //printf("%i\n",ppu.v);
            }
            real_time = epoch_nano()-start_nano;
            cpu_time = ns_wait*cpu->cycles;
            int diff = cpu_time-(real_time-paused_time);
            if (diff > 0) {
                std::this_thread::sleep_for(std::chrono::nanoseconds(diff));
            }
        }
        
    }
    
}

void NES::start() {
    running = true;
    running_t = std::thread( [this] { this->operation_thread(); } );
}

void NES::stop() {
    running = false;
}

py::array_t<uint8_t> NES::cpuMem() {
    uint8_t* tmp = (uint8_t*)cpu->memory;
    py::capsule cleanup(tmp, [](void *f){});
    return py::array_t<uint8_t>(
        {0x10000},
        {sizeof(uint8_t)},
        tmp,
        cleanup
    );
}

py::array_t<uint8_t> NES::getImg() {
    uint8_t* tmp = (uint8_t*)ppu->getImg();
    py::capsule cleanup(tmp,[](void *f){});
    return py::array_t<uint8_t>(
        {240,256,3},
        {sizeof(uint8_t)*256*3,sizeof(uint8_t)*3,sizeof(uint8_t)},
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
    .def("cpuMem",&NES::cpuMem)
    .def("getImg",&NES::getImg)
    .def("start",&NES::start)
    .def("stop",&NES::stop)
    .def("setController",&NES::setController);
    py::class_<ControllerWrapper>(m,"Controller").def(py::init<>())
    .def("updateInputs",&ControllerWrapper::updateInputs);

}
