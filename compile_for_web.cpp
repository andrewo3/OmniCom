#include <cstdio>
#include <sstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <windows.h>

int usage_error() {
    printf("Usage: nes2exec rom_filename\n");
    return -1;
}

void get_filename(char** path) {
    int l = strlen(*path);
    bool end_set = false;
    for (int i=l-1; i>=0; i--) {
        if ((*path)[i]=='.' && !end_set) {
            (*path)[i] = '\0';
            end_set = true;
        } else if ((*path)[i]=='/' || (*path)[i]=='\\') {
            *path = &(*path)[i+1];
            return;
        }
    }
}

std::string fixed(std::string input) {
    std::string output;
    for (char c: input) {
        if (c=='\\' || c==' ' || c=='[' || c==']' || c=='!') {
            output+='\\';
        }
        output+=c;
    }
    return output;
}

std::string xxdstring(std::string input) {
    std::string output;
    for (char c: input) {
        if (!isalnum(c)) {
            output+="_";
        } else {
            output+=c;
        }
    }
    return output;
}

int main(int argc, char ** argv) {
    if (argc!=2) {
        return usage_error();
    }
    std::stringstream command_stream;
    std::stringstream xxd_stream;
    xxd_stream << "xxd -i \"" << argv[1] << "\" > include/universal/rom_data.h";
     // Execute the command

    //change cwd (assuming script is running from path directory)
    //auto path = std::filesystem::current_path();
    char path[256];
    GetModuleFileName(NULL,path,256);
    std::filesystem::path cpath = path;
    std::cout << "EXEC: " << cpath << "\n";
    std::cout << "NEW: " << cpath.parent_path().parent_path() << "\n";
    auto original_path = std::filesystem::current_path();
    std::filesystem::current_path(cpath.parent_path().parent_path());
    std::system(xxd_stream.str().c_str());
    command_stream << "\"\"D:\\C++ Projects\\emsdk\\upstream\\emscripten\\em++\" -std=c++17 -O3 -sUSE_PTHREADS=1 -sPTHREAD_POOL_SIZE=4 -sASSERTIONS -sSTACK_SIZE=1048576 -sWASM_BIGINT=1 -lGLEW -DWEB=1 -DROM_NAME=\"\\\"" << fixed(argv[1]);
    command_stream << "\\\"\" -DDATAROM="<<xxdstring(argv[1])<<" -DDATALENGTH="<<xxdstring(argv[1])<<"_len ";
    get_filename(&argv[1]);
    command_stream << "-Isrc\\ntsc-filter -Iinclude\\universal -Isrc\\imgui -Isrc\\imgui\\backends src/imgui/backends/*.cpp src/imgui/*.cpp src/ntsc-filter/crt_core.c src/ntsc-filter/crt_ntsc.c src/*.cpp -Llib -sUSE_SDL=2 -o web/index.html\"";
    std::string command = command_stream.str();
    printf("%s\n",command.c_str());
    std::system("emsdk_env");
    int result = std::system(command.c_str());

    // Check the result
    if (result == 0) {
        printf("Command executed successfully.\n");
    } else {
        printf("Command execution failed with error code: %i\n", result);
    }
    return 0;
}