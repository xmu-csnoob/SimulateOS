#ifndef BLOCKS_H
#define BLOCKS_H
#include "memory.h"
#define BLOCKS_NUM 32
#define BLOCK_SIZE MEMORY_SIZE / BLOCKS_NUM

size_t block_table[BLOCKS_NUM];

void init_blocks(){
    for(size_t i = 0; i < BLOCKS_NUM; i++){
        block_table[i] = i * BLOCK_SIZE;
    }
}
#endif