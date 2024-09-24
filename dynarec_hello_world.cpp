#include <sys/mman.h>

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
typedef unsigned long uintptr_t;
typedef void(*func)();

void memcpy(void* dest, void* src, int sz) {
    char* writable = (char*)dest;
    char* readable = (char*)src;
    for (int i=0; i<sz; i++) {
        writable[i] = readable[i];
    }
}

int movEncode(uint8_t reg, uint64_t val,void* ptr, int offset) {
    for (int s=0; s<4; s++) {
        int shamt = s<<4; // specify shift amount within val
        uint32_t movk = reg&0x1f; // set movk register
        uint64_t shift_mask = (uint64_t)0xffff<<shamt;
        uint64_t val_seg = val&(shift_mask); //get specific 16 byte segment of val
        if (val_seg) {
            val_seg>>=shamt;
            val_seg<<=5;
            movk|=val_seg; //insert segment into movk instruction
            movk|=(s&0x3)<<21; // set shift for movk instruction
            movk|=0x1e5<<23; //set remaining movk metadata
            memcpy((char*)ptr+offset,&movk,sizeof(uint32_t)); //copy instruction into memory
            offset+=4; //increment offset
        }
    }
    return offset;
}

int strlen(const char* str) {
    return *str?strlen(str+1)+1:0;
}

int main() {
    int protections = PROT_READ | PROT_WRITE;
    int instructions = 20;

    //allocate
    void* mem = mmap(0L,sizeof(uint32_t)*instructions,protections,MAP_ANONYMOUS | MAP_PRIVATE,-1,0);
    
    if (mem==(void*)-1) {
        return -1;
    }
    const char* string = "Hello World!\n";
    int length = strlen(string);
    uint32_t syscall = 0xd4000001;
    uint32_t ret = 0xd65f03c0;
    int offset = 0;
    //write
    offset = movEncode(0,1,mem,offset); //send stdout destination
    offset = movEncode(1,(uintptr_t)string,mem,offset); //send string location
    offset = movEncode(2,length,mem,offset); //send string length
    offset = movEncode(16,4,mem,offset); //send write syscall
    memcpy((char*)mem+offset,&syscall,sizeof(uint32_t));
    offset+=4;
    memcpy((char*)mem+offset,&ret,sizeof(uint32_t));

    //make executable
    uint8_t failure = mprotect(mem,sizeof(uint32_t)*instructions,PROT_EXEC);
    if (failure) {
        return -1;
    }
    func exec = reinterpret_cast<func>(mem);
    exec();

    //free
    failure = munmap(mem,sizeof(uint32_t)*instructions);
    
    if (failure) {
        return -1;
    }
    return 0;
}