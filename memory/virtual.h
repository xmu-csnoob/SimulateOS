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

page_table_entry page_table[V_PAGE_NUMS];

size_t allocate_page(size_t physical_page, size_t virtual_page){
    if (virtual_page < 0 || virtual_page >= V_PAGE_NUMS || physical_page < 0 || physical_page >= BLOCKS_NUM) {
        printf("\nError: Invalid page number.\n");
        return -1;
    }

    if (page_table[virtual_page].valid) {
        printf("\nError: Virtual page already allocated.\n");
        return -1;
    }

    page_table[virtual_page].physical_page = physical_page;
    page_table[virtual_page].valid = 1;
    return 1;
}

size_t free_page(size_t virtual_page){
    if (virtual_page < 0 || virtual_page >= V_PAGE_NUMS) {
        printf("\nError: Invalid page number.\n");
        return -1;
    }

    if (!page_table[virtual_page].valid) {
        printf("\nError: Virtual page not allocated.\n");
        return -1;
    }

    page_table[virtual_page].valid = -1;
    page_table[virtual_page].physical_page = -1;
    return 1;
}





#endif