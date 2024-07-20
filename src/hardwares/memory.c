#include "memory.h"

BYTE memory[MEMORY_SIZE];

void init_memory(){
    for(size_t i = 0; i < MEMORY_SIZE; i++){
        memory[i] = 0x00;
    }
}

BYTE* access_physical_memory(size_t pos){
    if(pos >= 0 && pos < MEMORY_SIZE){
         return &memory[pos]; 
    }
    return NULL;
}

void assign_physical_memory(size_t pos, BYTE* data, size_t size){
    if(pos >= 0 && pos < MEMORY_SIZE){
        if(pos + size <= MEMORY_SIZE){
            memcpy(&memory[pos], data, size);
        }else{
            memcpy(&memory[pos], data, MEMORY_SIZE - pos);
            _WARN("assign_physical_memory: Data truncated as it exceeds memory boundary.");
        }
    }else{
        _ERROR("assign_physical_memory: Invalid memory pos for assignment.");
    }
}

void printMemory(){
    for(size_t i = 0; i < MEMORY_SIZE; i++){
        printf("%d", memory[i]);
    }
}