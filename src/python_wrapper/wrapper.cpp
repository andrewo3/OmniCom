#include "util.h"
#include "glob_const.h"
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
#include <filesystem>
#ifdef __WIN32__
#include "Shlobj.h"
#include "Windows.h"
#endif

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
        NES();
        ~NES();
        int mapper;
        long long start_nano;
        long long real_time = 0;
        py::array_t<uint8_t> cpuMem();
        py::array_t<uint8_t> ppuMem();
        py::array_t<uint8_t> OAM();
        py::array_t<uint8_t> getImg();
        py::array_t<uint8_t> color_lookup();
        py::bytes getAudio();
        void setController(ControllerWrapper& cont,int port);
        void start();
        void stop();
        void operation_thread();
        void save(int ind);
        bool load(int ind);
        void set_pause(bool paused);
        bool setSaveDir(std::string dir);
        std::string getSaveDir();
        void detectOS(char* ROM_NAME);
        std::string state_save_dir;
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

void NES::set_pause(bool p) {
    if (!p && paused) {
        paused_time += (epoch_nano()-start_nano)-real_time;
    }
    paused = p;
}

bool NES::setSaveDir(std::string dir) {
    if (std::filesystem::exists(dir)) {
        state_save_dir = dir;
        return true;
    } else {
        return false;
    }
}

std::string NES::getSaveDir() {
    return state_save_dir;
}

void NES::detectOS(char* ROM_NAME) {
    char* filename = new char[strlen(ROM_NAME)+1];
    char* original_start = filename;
    memcpy(filename,ROM_NAME,strlen(ROM_NAME)+1);
    get_filename(&filename);

    char* removed_spaces = new char[strlen(filename)+1];

    for (int i=0; i<strlen(filename); i++) {
        removed_spaces[i] = filename[i];
        if (removed_spaces[i]==' ') {
            removed_spaces[i] = '_';
        }
    }
    removed_spaces[strlen(filename)] = '\0';
    int os = -1;
    #ifdef __APPLE__
        config_dir = std::string(std::getenv("HOME"))+"/Library/Containers";
        sep = '/';
        printf("MACOS, %s\n", config_dir.c_str());
        os = 0;
    #endif
    #ifdef __WIN32__
        TCHAR appdata[MAX_PATH] = {0};
        SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, appdata);
        config_dir = std::string(appdata);
        sep = '\\';
        printf("WINDOWS, %s\n", config_dir.c_str());
        os = 1;
    #endif
    #ifdef __unix__
        config_dir = std::string(std::getenv("HOME"))+"/.config";
        sep = '/';
        printf("LINUX, %s\n", config_dir.c_str());
        os = 2;
    #endif
    bool load = false;
    if (os != -1) {
        config_dir+=sep;
        config_dir+=std::string("Nes2Exec");
        if (!std::filesystem::exists(config_dir)) { //make Nes2Exec appdata folder
            std::filesystem::create_directory(config_dir);
        }
        config_dir+=sep;
        config_dir+=std::string(removed_spaces);
        state_save_dir = config_dir;
        printf("%s\n",(config_dir).c_str());
        if (!std::filesystem::exists(config_dir)) { //make specific game save folder
            std::filesystem::create_directory(config_dir);
        } else {
            printf("Folder already exists. Checking for save...\n");
            if (std::filesystem::exists(config_dir+sep+std::string("state"))) {
                load = true;
            }
        }
    } else {
        printf("OS not detected. No save folder created.\n");
    }
}

NES::NES(char* rom_name) {
    detectOS(rom_name);
    rom = new ROM(rom_name);
    cpu = new CPU(false);
    apu = new APU();
    apu->setCPU(cpu);
    apu->sample_rate = 44100;
    cpu->apu = apu;
    cpu->loadRom(rom);
    cont1 = Controller();
    cont2 = Controller();
    cpu->set_controller(&cont1,0);
    cpu->set_controller(&cont2,1);
    cpu->reset();
    ppu = new PPU(cpu);


}

NES::NES() {
    printf("No rom specified.\n");
    rom = new ROM();
    printf("rom created.\n");
    cpu = new CPU(false);
    apu = new APU();
    apu->setCPU(cpu);
    apu->sample_rate = 44100;
    cpu->apu = apu;
    cpu->loadRom(rom);
    cont1 = Controller();
    cont2 = Controller();
    cpu->set_controller(&cont1,0);
    cpu->set_controller(&cont2,1);
    cpu->reset();
    ppu = new PPU(cpu);


}

void NES::save(int ind) {
    FILE* s = fopen((state_save_dir+sep+std::to_string(ind)).c_str(),"wb");
    cpu->save_state(s);
    fclose(s);
}

bool NES::load(int ind) {
    if (std::filesystem::exists(state_save_dir+sep+std::to_string(ind))) {
        FILE* s = fopen((state_save_dir+sep+std::to_string(ind)).c_str(),"rb");
        cpu->load_state(s);
        fclose(s);
        return true;
    } else {
        return false;
    }
}

void NES::setController(ControllerWrapper& cont, int port) {
    cpu->set_controller(&(cont.cont),port);
}

void NES::operation_thread() {
    
    const double ns_wait = 1e9/cpu->CLOCK_SPEED;
    long long cpu_time;
    paused_time = start_nano;
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
    start_nano = epoch_nano();
    cpu->start = start_nano;
    apu->start = start_nano;
    running_t = std::thread( [this] { this->operation_thread(); } );
}

void NES::stop() {
    if (cpu->rom->battery_backed) {
        std::FILE* ram_save = fopen((config_dir+sep+std::string("ram")).c_str(),"wb");
        cpu->save_ram(ram_save);
        fclose(ram_save);
    }
    running = false;
    running_t.join();
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

py::array_t<uint8_t> NES::color_lookup() {
    uint8_t* tmp = NTSC_TO_RGB;
    py::capsule cleanup(tmp, [](void *f){});
    return py::array_t<uint8_t>(
        {64,3},
        {sizeof(uint8_t)*3,sizeof(uint8_t)},
        tmp,
        cleanup
    );
}

py::array_t<uint8_t> NES::ppuMem() {
    uint8_t* tmp = (uint8_t*)ppu->memory;
    py::capsule cleanup(tmp, [](void *f){});
    return py::array_t<uint8_t>(
        {0x4000},
        {sizeof(uint8_t)},
        tmp,
        cleanup
    );
}

py::array_t<uint8_t> NES::OAM() {
    uint8_t* tmp = (uint8_t*)ppu->oam;
    py::capsule cleanup(tmp, [](void *f){});
    return py::array_t<uint8_t>(
        {0x100},
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

py::bytes NES::getAudio() {
    if (apu->queue_audio_flag) {
        apu->queue_audio_flag = false;
        return py::bytes((char *)apu->buffer_copy,BUFFER_LEN*sizeof(int16_t));
    } else {
        return py::bytes("");
    }
}

NES::~NES() {
    delete rom;
    delete cpu;
    delete ppu;
    delete apu;
}

PYBIND11_MODULE(pyNES,m) {
    py::class_<NES>(m,"NES").def(py::init<char*>()).def(py::init<>())
    .def("cpuMem",&NES::cpuMem)
    .def("ppuMem",&NES::ppuMem)
    .def("OAM",&NES::OAM)
    .def("getImg",&NES::getImg)
    .def("colorLookup",&NES::color_lookup)
    .def("getAudio",&NES::getAudio)
    .def("start",&NES::start)
    .def("stop",&NES::stop)
    .def("saveState",&NES::save)
    .def("loadState",&NES::load)
    .def("setPaused",&NES::set_pause)
    .def("setSaveDir",&NES::setSaveDir)
    .def("getSaveDir",&NES::getSaveDir)
    .def("setController",&NES::setController);
    py::class_<ControllerWrapper>(m,"Controller").def(py::init<>())
    .def("updateInputs",&ControllerWrapper::updateInputs);

} 
