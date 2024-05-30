//global desktop emulator stuff

#include "util.h"
#include "nes_sys.h"
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

uint8_t mapped_joy[8] = {0};

const uint8_t* state = SDL_GetKeyboardState(nullptr);
SDL_Joystick* controller = NULL;

//parameters
float global_volume = 50;
float global_db = 0.24;
bool use_shaders = false;
int changing_keybind = -1;
int render_engine = 0;
int current_device = 0;

bool paused = false;
bool paused_window = false;
int current_tab = 0;
static ImGuiWindowFlags paused_flags = ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoTitleBar;

int joystickDir(SDL_Joystick* joy) {
    float x = SDL_JoystickGetAxis(joy,0)/32768.0;
    float y = SDL_JoystickGetAxis(joy,1)/32768.0;
    if (x>y && x>sqrt(2)/2) {
        return SDL_HAT_RIGHT; //right
    } else if (x>y && y<-sqrt(2)/2) {
        return SDL_HAT_UP; //up
    } else if (x<y && x<-sqrt(2)/2) {
        return SDL_HAT_LEFT; //left
    } else if (x<y && y>sqrt(2)/2) {
        return SDL_HAT_DOWN; //down
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

int default_config() {
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
    return os;
}

void viewportBox(int** viewport_box,int width, int height, float aspect_ratio) {
    bool horiz = (float)width/height>aspect_ratio;
    (*viewport_box)[0] = horiz ? (width-aspect_ratio*height)/2.0 : 0;
    (*viewport_box)[1] = horiz ? 0 : (height-width/aspect_ratio)/2.0;
    (*viewport_box)[2] = horiz ? aspect_ratio*height : width;
    (*viewport_box)[3] = horiz ? height : width/aspect_ratio;
}

void setGLViewport(int width, int height, float aspect_ratio) {
    int* viewport = new int[4];
    viewportBox(&viewport,width,height,aspect_ratio);
    //printf("Viewport set\n");
    //printf("%i %i %i %i\n",viewport[0],viewport[1],viewport[2],viewport[3]);
    glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);
    delete[] viewport;
}

void pause_menu(BaseSystem* system) {
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
            if (!web) {
                if (ImGui::Button("Load")) {
                    printf("load game\n");
                    std::string load_dir = config_dir+sep+std::string("state");
                    if (std::filesystem::exists(load_dir)) {
                        FILE* save_file = fopen(load_dir.c_str(),"rb");
                        system->Load(save_file);
                        //cpu->load_state(save_file);
                        fclose(save_file);
                    } else {
                        printf("Nothing to load at: %s\n",load_dir.c_str());
                    }
                    
                }
                ImGui::SameLine(0);
                if (ImGui::Button("Save")) {
                    std::string load_dir = config_dir+sep+std::string("state");
                    printf("save game at: %s\n",load_dir.c_str());
                    FILE* save_file = fopen(load_dir.c_str(),"wb");
                    system->Save(save_file);
                    //cpu->save_state(save_file);
                    fclose(save_file);
                }
            }
            
            break;
        case 2:
            {
            int joy_count = SDL_NumJoysticks();
            const char* device_prev = current_device > 0 ? SDL_JoystickNameForIndex(current_device-1) : "Keyboard";
            if (ImGui::BeginCombo("Device", device_prev)) {
                if (ImGui::Selectable("Keyboard", current_device == 0)) {
                    printf("Set Controller Device to Keyboard\n");
                    if (controller!=NULL) {
                        SDL_JoystickClose(controller);
                    }
                    controller = NULL;
                    current_device = 0;
                }
                if (current_device == 0) {
                    ImGui::SetItemDefaultFocus();
                }
                for (int n = 0; n < SDL_NumJoysticks(); n++) {
                    const bool is_selected = (current_device == n+1);
                    if (ImGui::Selectable(SDL_JoystickNameForIndex(n), is_selected)) {
                        if (controller!=NULL) {
                            SDL_JoystickClose(controller);
                        }
                        controller = SDL_JoystickOpen(n);
                        printf("Set Controller Device to %s\n",SDL_JoystickNameForIndex(n));
                        current_device = n+1;
                    }
                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }

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
                std::string joystick_lbl;
                if (mapped_joy[i]&0x80) {
                    switch (mapped_joy[i]&0x7f) {
                        case SDL_HAT_UP:
                            joystick_lbl += "Up";
                            break;
                        case SDL_HAT_DOWN:
                            joystick_lbl += "Down";
                            break;
                        case SDL_HAT_LEFT:
                            joystick_lbl += "Left";
                            break;
                        case SDL_HAT_RIGHT:
                            joystick_lbl += "Right";
                            break;
                    }
                } else {
                    joystick_lbl += "Button "+std::to_string(mapped_joy[i]);
                }
                std::string labelID = "";
                labelID+=current_device > 0 ? joystick_lbl : SDL_GetScancodeName(mapped_keys[i]);
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
            }
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