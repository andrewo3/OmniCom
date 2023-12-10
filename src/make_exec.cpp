#include <cstdio>
#include <sstream>
#include <cstdlib>
#include <string>
#include <cstring>

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
    std::system(xxd_stream.str().c_str());
    command_stream << "g++ -DGLEW_STATIC -DROM_NAME=\"\\\"" << fixed(argv[1]);
    command_stream << "\\\"\" -DDATAROM="<<xxdstring(argv[1])<<" -DDATALENGTH="<<xxdstring(argv[1])<<"_len ";
    get_filename(&argv[1]);
    command_stream << "src/util.cpp src/rom.cpp src/cpu.cpp src/cpu_helper.cpp src/ppu.cpp src/apu.cpp -g src/main.cpp -Iinclude/universal -Llib -lglew32s -lSDL2 -lopengl32 -o \"bin/" << argv[1] << ".exe\" -mwindows";
    std::string command = command_stream.str();
    printf("%s\n",command.c_str());
    int result = std::system(command.c_str());

    // Check the result
    if (result == 0) {
        printf("Command executed successfully.\n");
    } else {
        printf("Command execution failed with error code: %i\n", result);
    }
    return 0;
}