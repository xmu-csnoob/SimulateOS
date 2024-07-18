#ifndef VIRTUAL_H
#define VIRTUAL_H
#include "memory.h"
#include "blocks.h"

#define V_MEMORY_SIZE 2048
#define V_PAGE_SIZE 32
#define V_PAGE_NUMS V_MEMORY_SIZE / V_PAGE_SIZE

typedef struct{
    size_t physical_page;
    size_t valid;
} page_table_entry;

extern page_table_entry page_table[V_PAGE_NUMS];

void init_pages();

size_t allocate_page(size_t physical_page, size_t virtual_page);

size_t free_page(size_t virtual_page);

#endif