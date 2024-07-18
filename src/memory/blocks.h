#ifndef BLOCKS_H
#define BLOCKS_H
#include "memory.h"
#define BLOCKS_NUM 32
#define BLOCK_SIZE MEMORY_SIZE / BLOCKS_NUM

extern size_t block_table[BLOCKS_NUM];

void init_blocks();

void print_block(size_t block);

#endif