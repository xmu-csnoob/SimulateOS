#ifndef MEMORY_H
#define MEMORY_H
#define MEMORY_SIZE 1024
#include "stdio.h"

extern unsigned char memory[MEMORY_SIZE];

void init_memory(){
    for(int i = 0; i < MEMORY_SIZE; i++){
        memory[i] = 0;
    }
}

unsigned char* access_memory(int pos){
    if(pos >= 0 && pos < MEMORY_SIZE){
         return &memory[pos]; 
    }
    return NULL;
}

void assign_memory(int pos, unsigned char* data, size_t size){
    if(pos >= 0 && pos < MEMORY_SIZE){
        if(pos + size <= MEMORY_SIZE){
            memcpy(&memory[pos], data, size);
        }else{
            memcpy(&memory[pos], data, MEMORY_SIZE - pos);
            printf("Warning: Data truncated as it exceeds memory boundary. \n");
        }
    }else{
        printf("Warning: Invalid memory pos for assignment");
    }
}

#endif