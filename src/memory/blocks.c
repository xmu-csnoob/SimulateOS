#include "blocks.h"

size_t block_table[BLOCKS_NUM];

void init_blocks(){
    for(size_t i = 0; i < BLOCKS_NUM; i++){
        block_table[i] = i * BLOCK_SIZE;
    }
}

void print_block(size_t block){
    size_t start = block_table[block];
    for(int i = start; i < start + BLOCK_SIZE; i++){
        printf("%X", memory[i]);
    }
    printf("\n");
}