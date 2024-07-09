#ifndef PAGE_H
#define PAGE_H
#include "memory.h"
#define VIRTUAL_MEMORY_SIZE 2048
#define PAGE_SIZE 64
#define PAGE_NUMS VIRTUAL_MEMORY_SIZE / PAGE_SIZE
BYTE virtual_memory[VIRTUAL_MEMORY_SIZE];
int page_table[PAGE_NUMS];
#endif