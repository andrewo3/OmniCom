#include "util.h"
#include "SDL2/SDL.h"
#include "math.h"
#include "imgui.h"
#include <string>
#include <filesystem>
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "cpu.h"
#include "controller.h"

GLfloat vertices[16] = {
        -1.0f, 1.0f,0.0f,0.0f,
        -1.0f, -1.0f,0.0f,1-0.04f*use_shaders,
        1.0f, -1.0f,1.0f,1-0.04f*use_shaders,
        1.0f, 1.0f, 1.0f,0.0f
    };

//currently mapped keys - set to A,B,Select,Start,Up,Down,Left,Right.
//in the (reverse) order that the CPU reads at $4016
SDL_Scancode mapped_keys[8] = {
    SDL_SCANCODE_SPACE,
    SDL_SCANCODE_LSHIFT,
    SDL_SCANCODE_TAB,
    SDL_SCANCODE_RETURN,
    SDL_SCANCODE_UP,
    SDL_SCANCODE_DOWN,
    SDL_SCANCODE_LEFT,
    SDL_SCANCODE_RIGHT};

const uint8_t* state = SDL_GetKeyboardState(nullptr);
SDL_Joystick* controller = NULL;

//parameters
float global_volume = 50;
float global_db = 0.24;
bool use_shaders = false;
int changing_keybind = -1;
int render_engine = 0;


bool paused_window = false;
int current_tab = 0;
static ImGuiWindowFlags paused_flags = ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoTitleBar;

int joystickDir(SDL_Joystick* joy) {
    float x = SDL_JoystickGetAxis(joy,0)/32768.0;
    float y = SDL_JoystickGetAxis(joy,1)/32768.0;
    if (x>y && x>sqrt(2)/2) {
        return 0; //right
    } else if (x>y && y<-sqrt(2)/2) {
        return 3; //up
    } else if (x<y && x<-sqrt(2)/2) {
        return 2; //left
    } else if (x<y && y>sqrt(2)/2) {
        return 1; //down
    }
    return -1;
}

static std::string labelPrefix(const char* const label)
{
	float width = ImGui::CalcItemWidth();

	float x = ImGui::GetCursorPosX();
	ImGui::Text(label); 
	ImGui::SameLine(); 
	//ImGui::SetCursorPosX(x + width * 0.5f + ImGui::GetStyle().ItemInnerSpacing.x);
	//ImGui::SetNextItemWidth(-1);

	std::string labelID = "##";
	labelID += label;

	return labelID;
}

int tab_offset = 10;
int menu_buttons = 5;

void MenuButton(ImVec2 win_size, int number,char* name) {
    bool active = false;
    if (current_tab==number) {
        ImGui::PushStyleColor(ImGuiCol_Button,ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
        active = true;
    }
    if (ImGui::Button(name,ImVec2((win_size.x-tab_offset*2)/menu_buttons,20))) {
        current_tab = number;
    }
    if (active) {
        ImGui::PopStyleColor();
    }
}

void pause_menu(void** system) {
    CPU* cpu = (CPU*)system[0];
    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImVec2 size = main_viewport->WorkSize;
    ImGui::SetNextWindowPos(main_viewport->WorkPos);
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowBgAlpha(0.5f);

    ImGui::Begin("Pause Menu",&paused_window,paused_flags);
    ImGui::SetCursorPosX(10);

    MenuButton(size,0,"General");
    ImGui::SameLine(0,0);
    MenuButton(size,1,"Debug");
    ImGui::SameLine(0,0);
    MenuButton(size,2,"Controls");
    ImGui::SameLine(0,0);
    MenuButton(size,3,"Video");
    ImGui::SameLine(0,0);
    MenuButton(size,4,"Audio");
    ImGui::SetCursorPosY(size.y/4);
    char * button_names[8] = {
        "A",
        "B",
        "Select",
        "Start",
        "Up",
        "Down",
        "Left",
        "Right"

    };
    switch (current_tab) {
        case 0:
            /*if (ImGui::Button("Load")) {
                printf("load game\n");
                std::string load_dir = config_dir+sep+std::string("state");
                if (std::filesystem::exists(load_dir)) {
                    FILE* save_file = fopen(load_dir.c_str(),"rb");
                    cpu->load(save_file);
                    fclose(save_file);
                } else {
                    printf("Nothing to load at: %s\n",load_dir.c_str());
                }
                
            }*/
            /*ImGui::SameLine(0);
            if (ImGui::Button("Save")) {
                std::string load_dir = config_dir+sep+std::string("state");
                printf("save game at: %s\n",load_dir.c_str());
                cpu->save();
            }*/
            
            break;
        case 2:
            for (int i=0; i<8; i++) {
                ImGui::Text(button_names[i]);
                ImGui::SameLine(0);
                ImGui::SetCursorPosX(size.x*(i%2)/2+50);
                bool active = false;
                if (changing_keybind>-1 && i==changing_keybind) {
                    ImGui::PushStyleColor(ImGuiCol_Button,ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
                    active = true;
                }
                std::string labelID = "";
                labelID+=SDL_GetScancodeName(mapped_keys[i]);
                labelID+="##";
                labelID+='0'+i;
                if (ImGui::Button(labelID.c_str(),ImVec2(100,50)) && changing_keybind==-1) {
                    changing_keybind = i;
                }
                if (active) {
                    ImGui::PopStyleColor(2);
                }
                if (!(i&1)) {
                    ImGui::SameLine(0);
                    ImGui::SetCursorPosX(size.x/2);
                }
            }
            break;
        case 3:
            {
            ImGui::Checkbox(labelPrefix("Use NTSC Filter").c_str(), &use_shaders);
            ImGui::Text("Rendering Engine");
            ImGui::RadioButton("OpenGL",&render_engine,0);
            ImGui::SameLine();
            ImGui::RadioButton("Vulkan",&render_engine,1);
            break;
            }
        case 4:
            ImGui::SliderFloat(labelPrefix("Volume").c_str(),&global_volume,0.0,100.0,"%.0f%",0);
            global_db = (powf(10,global_volume/100)-1)/9;
            break;
    }
    ImGui::End();
}