# Nes2Exec Design

This website: [NesDev.org](https://www.nesdev.org/wiki/Nesdev_Wiki), has been incredibly valuable to me in making this project. Almost all of the technical info I used to make it came from there.

This project consists of 3 main components (one of which is not functional right now). Those components are the CPU, PPU, and APU. The CPU controls the game logic, the PPU controls the picture displayed to the screen, and the APU controls the audio. These 3 components work in tandem alongside the main window thread (setup using ```std::thread```) to emulator the game fed into the program. I'll start my explanation with the main application.

## Main Application

The program starts by initializing a bunch of global variables in chunks, and by the time ```main()``` is called, I have this:

```cpp
if (argc!=2) {
    return usage_error();
}
ROM rom(argv[1]);
if (!rom.is_valid()) {
    return invalid_error();
}

printf("Mapper: %i\n",rom.get_mapper()); //https://www.nesdev.org/wiki/Mapper
printf("Mirrormode: %i\n",rom.mirrormode);

char* filename = new char[strlen(argv[1])+1];
char* original_start = filename;
memcpy(filename,argv[1],strlen(argv[1])+1);
get_filename(&filename);
```
This checks to see if you have specified a rom to input into the program. If you didn't, it exits, and tells you how to use it properly. If you did, it will try to read it into a ```ROM``` object, which I defined in ```rom.cpp```. ```ROM``` objects are identified by the beginning 4 bytes, which should read out ```NES```, followed by ```0x1A```. 
```cpp
this->src_filename = src;
filename_length = strlen(src);
FILE* rp = std::fopen(src_filename,"rb"); // rom pointer
std::fread(header,16,1,rp);
if (header[0]=='N' && header[1]=='E' && header[2]=='S' && header[3]==0x1a) {
    valid_rom = true;
} else {
    return;
}
```

## ROMs

Once that is identified, the ```ROM``` object checks the remaining 12 bytes, which, combined with the ```NES 0x1A```, make up the "header" of the rom file. This has all of the important information about the ROM, most important of which is the size of the "PRG-ROM" (program rom), and the "CHR-ROM" (character ROM). These are just subsets of the ROM which each contain game code and sprites, respectively. The PRG and CHR ROM (almost) always immediately follow the header (except in the case where there is a trainer, which is like hacked code that I didn't think was necessary to handle for this project). The ROM class reads the PRG and CHR ROMs into separate malloc'ed arrays, (freed when the ROM is destructed), and later on the CPU and PPU are able to access it and load them into the respective portions in memory according to the mapper for the ROM.

### Mapper

Put simply the mapper is just a piece of hardware attached to the cartridge (and subsequently the ROM) that changes the way the data is loaded to and from the ROM, allowing the ROM to have more bytes than the alloted CPU/PPU space for data - sectioning it off into "banks". For the purposes of this project I only implemented "mapper 0", which is the simplest of them all - Mapper 0 loads the CPU rom at $8000, mirroring it into $C000 as well if it is only 16kb, and allowing the whole $8000-$FFFF to fill up if it is less

After the ROM is initialized, the main thread goes to initialize SDL, audio (which I will skip because it is not functional at the moment), and OpenGL:
```cpp
// SDL initialize
SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
SDL_ShowCursor(0);
printf("SDL Initialized\n");

//set display dimensions
SDL_DisplayMode DM;
SDL_GetDesktopDisplayMode(0,&DM);
WINDOW_INIT[0] = DM.w;
WINDOW_INIT[1] = DM.h;
SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 3);
int* viewport = new int[4];
viewportBox(&viewport,WINDOW_INIT[0],WINDOW_INIT[1]);
SDL_Window* window = SDL_CreateWindow(filename,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WINDOW_INIT[0],WINDOW_INIT[1],FLAGS);
delete[] original_start;
printf("Window Created\n");
SDL_GLContext context = SDL_GL_CreateContext(window);
glewExperimental = GL_TRUE;
glewInit();
glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);
printf("OpenGL Initialized.\n");
GLenum error = glGetError();
```

SDL (short for Simple DirectMedia Layer), is a library that lets you make graphics applications for your computer. This is the library that lets me actually make a window to display information on. It is able to communicate with OpenGL, by setting the ```SDL_WINDOW_OPENGL``` flag on a window. OpenGL is a graphics API that allows you to write code that interfaces with your graphics card to draw graphics to the screen. It wasn't strictly necessary for me to use OpenGL for this project, but I wanted to because I could write shaders, which can modify what is drawn to the screen in fun ways. There are vertex and fragment shaders which I initialize later in the main loop. Vertex shaders modify vertex information, changing where things are on the screen, and fragment shaders change the actual colors of the pixels on the screen, based on the given location and color information. See ```res/roms/shaders``` to find my vertex and fragment shaders, which I ultimately hardcoded into memory in ```include/universal/shader_data.h``` with the ```xxd``` command on Linux to remove the need to actually have the shader files present with your final compiled executable.

After that I initialize a texture image with OpenGL, which is just a global variable called ```out_img```, which is constantly updated by the PPU in order to produce a coherent image.

I then initialize the CPU and PPU (skipping APU because it doesn't work). 

## CPU
Without explaining line by line, the```CPU``` class has a lookup table defined in the ```CPU::define_opcodes()``` function in ```cpu_helper.cpp```, that takes every (valid) opcode for the CPU, where an opcode is just a number that corresponds to an instruction, and puts a pointer to the function that defines the instruction into an array called ```opcodes```. There is also another array called ```addrmodes```. On the NES CPU, which is called the MOS 6502, there are 56 opcodes which each have unique "addressing modes". Addressing modes modify the value passed into the function, and change where its memory location is. I can't explain each addressing mode and each instruction here, but they are all defined in cpu_helper.cpp, and the instructions are in alphabetical order, and they are each very short, and easy to understand individually. The CPU has three flags as well - X, Y, and the Accumulator, which are each just 3 numbers that represent different things. After performing operations on the CPU, the flags are updated. The flags are represented by a single number, where each bit is a different flag. Flags are updated after each function in different ways, which are all defined on [here](https://www.nesdev.org/obelisk-6502-guide/reference.html).

The CPU starts reading instructions at the address pointed to at $FFFC (where FFFC is a hexadecimal address in memory, which I represented as an array with size 10000 also in hexadecimal), and it increments it's program counter as it reads along (this happens in ```CPU::clock()```).

When the CPU accesses to specific addresses (mostly located around $2000, where "2000" is in hexadecimal), these allow the CPU to communicate with the PPU. These all have different functions defined [here](https://www.nesdev.org/wiki/PPU_registers).


## PPU 
The PPU is a separate chip on the NES that is less complex (although still very complex). It constantly goes through 261 scanlines (just rows of 256 pixels) - 240 of which are actually visible on the screen and the last 21 are part of the "vertical blank" (a portion of the rendering pipeline that is used for updating cpu logic and other set up things for the PPU) - and draws values to the screen. It splits rendering up into background and sprites. 

Background rendering uses a portion of the memory called a nametable - it contains the index for the patterns used for that specific tile on the background. It uses another table called an attribute table to decide which palette of colors to use for that tile.

Sprite rendering is similar, it uses a separate portion of memory called OAM within the PPU to make a list of sprites on the screen. It copies them into another smaller portion of memory called secondary OAM to determine which sprites need to be drawn on the specific scanline.

It then loops through all of the sprites for each pixel on the scanline and determines if the sprites need to be drawn in front or behind the background, and sets the pixel accordingly. This is all done in the ```PPU::cycle()``` function.

Once it finishes calculating, it draws to the screen by writing the pixel to the ```out_img``` variable from ```util.cpp```, and that is drawn to the screen back in the main thread. The little face I have in the res folder in ```res/test_image.jpg``` was an image I used to test drawing with OpenGL.

## Render loop

The render loop is comparatively much simpler. It checks for events (really just closing the window), and then uses the shaders with OpenGL to draw the out_img variable to the screen. It does this by setting the out_img variable to a texture using ```glTexImage2D()```, setting the format and everything necessary, then activates the texture and the shader program (which consists of both the vertex and fragment shaders), and then updates the screen with it by clearing the buffer. When the window is closed, or when a keyboard interrupt (or segfault) is detected by the ```std::signal()``` function, the ```quit()``` function runs and tells the user in the command line what the effective clock speed was and how it compares to the actual clock speed of the original 6502.