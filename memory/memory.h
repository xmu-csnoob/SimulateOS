#ifndef MEMORY_H
#define MEMORY_H
#define MEMORY_SIZE 1024
#include <stdio.h>
#include <string.h>

typedef unsigned char BYTE;

BYTE memory[MEMORY_SIZE];

#define MEMORY_START_ADDRESS 0
#define MEMORY_END_ADDRESS MEMORY_SIZE - 1

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
            printf("Warning: Data truncated as it exceeds memory boundary. \n");
        }
    }else{
        printf("Error: Invalid memory pos for assignment. \n");
    }
}

void printMemory(){
    for(size_t i = 0; i < MEMORY_SIZE; i++){
        printf("%d", memory[i]);
    }
}

#endif